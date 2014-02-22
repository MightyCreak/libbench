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

#include <thread>
#include "benchmark.h"
#include "thread.h"

namespace bench
{
    BenchMark::BenchMark()
    {
        unsigned int numCores = std::thread::hardware_concurrency();
        m_coreNames.reserve(numCores);
        for(unsigned int u = 0; u < numCores; ++u)
        {
            char coreName[30];
            sprintf(coreName, "Core #%d", u + 1);
            m_coreNames.push_back(coreName);
        }
    }

	BenchMark::~BenchMark()
	{
		Clear();
	}

	unsigned int BenchMark::GetNumberOfCores() const
	{
	    return m_coreNames.size();
	}
	
	void BenchMark::SetCoreName(unsigned int id, char const* name)
	{
	    m_coreNames[id] = name;
	}
	
	char const* BenchMark::GetCoreName(unsigned int id) const
	{
	    return m_coreNames[id].c_str();
	}

	ThreadId BenchMark::AddThread(char const* name, int coreId)
	{
		Thread* thread = new Thread(name, coreId);
		m_threads.push_back(thread);
		return m_threads.size() - 1;
	}

	Thread* BenchMark::GetThread(ThreadId id) const
	{
		return m_threads[id];
	}

	ThreadVector const& BenchMark::GetThreadVector() const
	{
		return m_threads;
	}

	void BenchMark::Finalize()
	{
		for (ThreadVector::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
		{
			Thread* thread = *it;
			thread->Finalize();
		}
	}

	void BenchMark::Clear()
	{
		for (ThreadVector::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
		{
			Thread* thread = *it;
			thread->Clear();
			delete thread;
		}

		m_threads.clear();
	}
}
