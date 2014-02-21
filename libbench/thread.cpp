/*
 * thread.cpp
 *
 *  Created on: 2013-09-25
 *      Author: creak
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"

namespace libbench
{
	Thread::Thread(char const* name, int coreId)
		: m_name(strdup(name))
		, m_coreId(coreId)
	{
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

    int Thread::GetCoreId() const
    {
        return m_coreId;
    }

    BenchList const& Thread::GetBenchList() const
    {
        return m_benchList;
    }

	void Thread::StartBench(char const* benchName)
	{
		m_benchList.StartBench(benchName);
	}

	void Thread::StopBench()
	{
		m_benchList.StopBench();
	}

	void Thread::Finalize()
	{
		m_benchList.Finalize();
	}

	void Thread::Clear()
	{
		m_benchList.Clear();
	}
}
