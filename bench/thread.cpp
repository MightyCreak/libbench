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
#include "thread.h"

namespace bench
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
