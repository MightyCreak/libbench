/*
 * benchmark.cpp
 *
 *  Created on: 2013-10-12
 *      Author: creak
 */

#include <thread>
#include "benchmark.h"
#include "thread.h"

namespace libbench
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
