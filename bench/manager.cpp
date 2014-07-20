/*
 * Copyright (C) 2014 Romain "Creak" Failliot.
 *
 * This file is part of libbench.
 *
 * libbench is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libbench is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libbench. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <assert.h>
#include <thread>
#include "manager.h"
#include "core.h"
#include "thread.h"
#include "xmlcommon.h"
#include "xmlwriter.h"

namespace
{
    std::thread::id GetThreadId()
    {
        return std::this_thread::get_id();
    }
}

namespace bench
{
    Manager * Manager::ms_instance = nullptr;

    void Manager::CreateInstance()
    {
        assert(!ms_instance);
        ms_instance = new Manager;
    }

    void Manager::DestroyInstance()
    {
        assert(ms_instance);
        delete ms_instance;
        ms_instance = nullptr;
    }

    Manager * Manager::GetInstance()
    {
        return ms_instance;
    }

    Manager::Manager()
    {
        unsigned int numCores = std::thread::hardware_concurrency();
        m_cores.reserve(numCores);
        for(unsigned int u = 0; u < numCores; ++u)
        {
            Core* core = new Core;
            assert(core);
            char coreName[30];
            sprintf(coreName, "Core #%d", u + 1);
            core->SetName(coreName);
            m_cores.push_back(core);
        }
    }

    Manager::~Manager()
    {
        for(Core* core : m_cores)
        {
            delete core;
        }
        m_cores.clear();
    }

    void Manager::SetCoreName(unsigned int core, char const* name)
    {
        assert(core < m_cores.size());
        m_cores[core]->SetName(name);
    }

    void Manager::SetThreadName(char const* name)
    {
        Thread* thread = GetOrRegisterThread();
        thread->SetName(name);
    }

    void Manager::StartBench(char const * name)
    {
        // Start bench.
        Thread* thread = GetOrRegisterThread();
        thread->StartBench(name);
    }

    void Manager::StopBench()
    {
        // Get current thread.
        m_mutex.lock();
        std::thread::id thrdId = GetThreadId();
        IdThreadMap::iterator it = m_threads.find(thrdId);
        assert(it != m_threads.end());
        Thread* thread = it->second;
        m_mutex.unlock();

        // Stop bench.
        thread->StopBench();
    }

    void Manager::Clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for(Core* core : m_cores)
        {
            delete core;
        }
        m_cores.clear();
        m_threads.clear();
    }

    void Manager::Write(std::ostream & stream) const
    {
        m_mutex.lock();

        // Find min start time.
        bool first = true;
        __time_t starttime = 0;
        for(Core const* core : m_cores)
        {
            for(Thread const* thread : core->m_threads)
            {
                for(Thread::Bench const& bench : thread->m_benches)
                {
                    if(first)
                    {
                        starttime = bench.m_start.tv_sec;
                        first = false;
                    }
                    else
                    {
                        starttime = std::min(starttime, bench.m_start.tv_sec);
                    }
                }
            }
        }

        Document doc;
        doc.m_cores.reserve(m_cores.size());
        for(Core const* core : m_cores)
        {
            doc.m_cores.push_back(DocumentCore());
            DocumentCore& docCore = doc.m_cores.back();
            docCore.m_name = core->GetName();

            docCore.m_threads.reserve(core->m_threads.size());
            for(Thread const* thread : core->m_threads)
            {
                docCore.m_threads.push_back(DocumentThread());
                DocumentThread& docThread = docCore.m_threads.back();
                char const* threadName = thread->GetName();
                if(threadName)
                {
                    docThread.m_name = thread->GetName();
                }

                ConvertBenchChildren(*thread, Thread::kNoParent, docThread.m_benches, starttime);
            }
        }

        m_mutex.unlock();

        XmlWriter xmlWriter(stream);
        xmlWriter.Write(doc);
    }

    void Manager::ConvertBenchChildren(Thread const& thread,
                                       int parentIdx,
                                       std::vector<DocumentBench>& docBenches,
                                       __time_t starttime) const
    {
        docBenches.reserve(500);
        size_t benchIdx = 0;
        for(Thread::Bench const& bench : thread.m_benches)
        {
            if(bench.m_parent == parentIdx)
            {
                docBenches.push_back(DocumentBench());
                DocumentBench& docBench = docBenches.back();

                // Get name.
                docBench.m_name = thread.GetBenchName(bench.m_nameId);

                // Get start-stop times.
                docBench.m_start = (bench.m_start.tv_sec - starttime) + bench.m_start.tv_nsec * 1e-9;
                docBench.m_stop = (bench.m_stop.tv_sec - starttime) + bench.m_stop.tv_nsec * 1e-9;

                ConvertBenchChildren(thread, benchIdx, docBench.m_benches, starttime);
            }
            ++benchIdx;
        }
        docBenches.shrink_to_fit();
    }

    Thread* Manager::GetOrRegisterThread()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        std::thread::id thrdId = GetThreadId();
        IdThreadMap::iterator it = m_threads.find(thrdId);
        Thread* thread;
        if(it != m_threads.end())
        {
            thread = it->second;
        }
        else
        {
            thread = RegisterThread(thrdId);
        }

        return thread;
    }

    Thread* Manager::RegisterThread(std::thread::id threadId)
    {
        int coreId = sched_getcpu();
        assert(coreId >= 0 && static_cast<size_t>(coreId) < m_cores.size());
        Core* core = m_cores[coreId];
        Thread* thread = core->AddThread();
        assert(thread);
        m_threads.insert(IdThreadMap::value_type(threadId, thread));
        return thread;
    }
}
