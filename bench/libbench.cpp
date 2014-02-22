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
#include <pthread.h>
#include <sched.h>
#include "libbench.h"
#include "thread.h"
#include "xmlwriter.h"
#include "config.h"

namespace
{
	pthread_t GetThreadId()
	{
		return pthread_self();
	}

	void GetThreadName(pthread_t thread, char * name, size_t maxsize)
	{
		pthread_getname_np(thread, name, maxsize);
	}
}

namespace libbench
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
		pthread_mutex_init(&m_mutex, nullptr);
	}

	Manager::~Manager()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	ThreadId Manager::RegisterThread(pthread_t thread)
	{
		char name[500];
		GetThreadName(thread, name, 500);
		ThreadId threadId = m_benchmark.AddThread(name, sched_getcpu());
		std::pair<ThreadIdMap::iterator, bool> res = m_threadIds.insert(ThreadIdMap::value_type(thread, threadId));
		return res.second ? res.first->second : kInvalidThread;
	}

	void Manager::StartBench(char const * benchName)
	{
		pthread_mutex_lock(&m_mutex);
		pthread_t pthrd = GetThreadId();
		ThreadIdMap::iterator it = m_threadIds.find(pthrd);
		ThreadId threadId;
		if(it == m_threadIds.end())
		{
			threadId = RegisterThread(pthrd);
		}
		else
		{
			threadId = it->second;
		}
		pthread_mutex_unlock(&m_mutex);

		Thread* thread = m_benchmark.GetThread(threadId);
		thread->StartBench(benchName);
	}

	void Manager::StopBench()
	{
		pthread_mutex_lock(&m_mutex);
		pthread_t pthrd = GetThreadId();
		ThreadIdMap::iterator it = m_threadIds.find(pthrd);
		assert(it != m_threadIds.end());
		Thread* thread = m_benchmark.GetThread(it->second);
		pthread_mutex_unlock(&m_mutex);
		thread->StopBench();
	}

	void Manager::Finalize()
	{
		pthread_mutex_lock(&m_mutex);
		m_benchmark.Finalize();
		pthread_mutex_unlock(&m_mutex);
	}

	void Manager::Clear()
	{
		pthread_mutex_lock(&m_mutex);
		m_benchmark.Clear();
		pthread_mutex_unlock(&m_mutex);
	}

	void Manager::Write(std::ostream & stream) const
	{
		XmlWriter xmlWriter(stream);
		pthread_mutex_lock(&m_mutex);
		xmlWriter.Write(m_benchmark);
		pthread_mutex_unlock(&m_mutex);
	}

    unsigned int GetMajorVersion()
    {
        return LIBBENCH_VERSION_MAJOR;
    }

    unsigned int GetMinorVersion()
    {
        return LIBBENCH_VERSION_MINOR;
    }
}
