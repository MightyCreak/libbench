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

#ifndef LIBBENCH_H_
#define LIBBENCH_H_

#include <map>
#include <ostream>
#include "benchmark.h"

namespace libbench
{
	typedef std::map<pthread_t, ThreadId> ThreadIdMap;

	class Manager
	{
	public:
		static void CreateInstance();
		static void DestroyInstance();
		static Manager* GetInstance();

		~Manager();

		void StartBench(char const* benchName);
		void StopBench();
		void Finalize();
		void Clear();
		void Write(std::ostream& stream) const;

	private:
		Manager();

		ThreadId RegisterThread(pthread_t thread);

	private:
		static Manager* ms_instance;

		BenchMark m_benchmark;
		ThreadIdMap m_threadIds;
		mutable pthread_mutex_t m_mutex;
	};

	class ScopedBench
	{
	public:
		ScopedBench(char const* name)
		{
			libbench::Manager::GetInstance()->StartBench(name);
		}

		~ScopedBench()
		{
			libbench::Manager::GetInstance()->StopBench();
		}
	};

    unsigned int GetMajorVersion();
    unsigned int GetMinorVersion();
}

#endif /* LIBBENCH_H_ */
