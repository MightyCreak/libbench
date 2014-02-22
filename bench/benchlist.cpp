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
#include <string.h>
#include <time.h>
#include <algorithm>
#include <boost/crc.hpp>
#include "benchlist.h"

namespace
{
    void GetTime(timespec &tsp)
    {
        clock_gettime(CLOCK_REALTIME, &tsp);
    }

    struct SortBenchByStartHieAndTime
    {
        bool operator () (bench::Bench const & a, bench::Bench const & b)
        {
            if(a.m_parent < b.m_parent)
                return true;
            if(a.m_parent > b.m_parent)
                return false;
            if(a.m_start.tv_sec < b.m_start.tv_sec)
                return true;
            if(a.m_start.tv_sec > b.m_start.tv_sec)
                return false;
            if(a.m_start.tv_nsec <= b.m_start.tv_nsec)
                return true;
            return false;
        }
    };
}

namespace bench
{
    int const BenchList::kNoParent = -1;

    BenchList::BenchList()
        : m_curNode(kNoParent)
    {
        m_benches.reserve(100000);
    }

    BenchVector const& BenchList::GetBenches() const
    {
        return m_benches;
    }

    char const* BenchList::GetBenchName(uint32_t id) const
    {
        StringMap::const_iterator it = m_benchNames.find(id);
        return it != m_benchNames.end() ? it->second : nullptr;
    }

    void BenchList::StartBench(char const * benchName)
    {
        Bench& bench = AddBench(benchName);
        GetTime(bench.m_start);
        bench.m_stop = bench.m_start;
        m_curNode = m_benches.size() - 1;
    }

    void BenchList::StopBench()
    {
        assert(m_curNode != BenchList::kNoParent);
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

    void BenchList::Finalize()
    {
        std::sort(m_benches.begin(), m_benches.end(), SortBenchByStartHieAndTime());
    }

    void BenchList::Clear()
    {
        m_benches.clear();
        for (StringMap::iterator it = m_benchNames.begin(); it != m_benchNames.end(); ++it)
            free(it->second);
        m_benchNames.clear();
    }

    Bench& BenchList::AddBench(char const * benchName)
    {
        boost::crc_32_type crc;
        crc.process_bytes(benchName, strlen(benchName));
        uint32_t id = crc.checksum();

        StringMap::const_iterator it = m_benchNames.find(id);
        if(it == m_benchNames.end())
            m_benchNames.insert(std::pair<uint32_t, char *>(id, strdup(benchName)));

        m_benches.push_back(Bench());
        Bench & bench = m_benches.back();
        bench.m_parent = m_curNode;
        bench.m_nameId = id;
        return bench;
    }
}
