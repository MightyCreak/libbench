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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <boost/crc.hpp>
#include "thread.h"

namespace
{
    void GetTime(timespec &tsp)
    {
        clock_gettime(CLOCK_REALTIME, &tsp);
    }
}

namespace bench
{
    int const Thread::kNoParent = -1;

    Thread::Thread(char const* name)
        : m_name(strdup(name))
        , m_curNode(kNoParent)
    {
        m_benches.reserve(100000);
    }

    Thread::~Thread()
    {
        if(m_name)
        {
            free(m_name);
            m_name = nullptr;
        }
    }

    char const* Thread::GetName() const
    {
        return m_name;
    }

    char const* Thread::GetBenchName(uint32_t id) const
    {
        StringMap::const_iterator it = m_benchNames.find(id);
        return it != m_benchNames.end() ? it->second : nullptr;
    }

    void Thread::StartBench(char const* name)
    {
        Bench& bench = AddBench(name);
        GetTime(bench.m_start);
        bench.m_stop = bench.m_start;
        m_curNode = m_benches.size() - 1;
    }

    void Thread::StopBench()
    {
        assert(m_curNode != Thread::kNoParent);
        Bench & bench = m_benches[m_curNode];
        GetTime(bench.m_stop);
        m_curNode = bench.m_parent;
        if(bench.m_stop.tv_sec == bench.m_start.tv_sec &&
                bench.m_stop.tv_nsec - bench.m_start.tv_nsec < 1000)
        {
            if((size_t) (m_curNode + 1) < m_benches.size())
                bench = m_benches.back();
            m_benches.pop_back();
        }
    }

    void Thread::Finalize()
    {
        //m_benchList.Finalize();
    }

    void Thread::Clear()
    {
        //m_benchList.Clear();
    }

    Thread::Bench& Thread::AddBench(char const * name)
    {
        boost::crc_32_type crc;
        crc.process_bytes(name, strlen(name));
        uint32_t id = crc.checksum();

        StringMap::const_iterator it = m_benchNames.find(id);
        if(it == m_benchNames.end())
        {
            m_benchNames.insert(std::pair<uint32_t, char *>(id, strdup(name)));
        }

        m_benches.push_back(Bench());
        Bench & bench = m_benches.back();
        bench.m_parent = m_curNode;
        bench.m_nameId = id;
        return bench;
    }
}
