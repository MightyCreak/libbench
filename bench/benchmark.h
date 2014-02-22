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

#ifndef BENCH_BENCHMARK_H
#define BENCH_BENCHMARK_H

#include <vector>
#include <cstddef>

namespace bench
{
    typedef std::vector<std::string> StringVector;

	class Thread;
	typedef std::vector<Thread*> ThreadVector;

	typedef size_t ThreadId;

	static ThreadId const kInvalidThread(~0);

	class BenchMark
	{
	public:
	    BenchMark();
		~BenchMark();
		
		unsigned int GetNumberOfCores() const;
		void SetCoreName(unsigned int id, char const* name);
		char const* GetCoreName(unsigned int id) const;

		ThreadId AddThread(char const* name, int coreId);
		Thread* GetThread(ThreadId id) const;
		ThreadVector const& GetThreadVector() const;
		void Finalize();
		void Clear();

	private:
	    StringVector m_coreNames;
		ThreadVector m_threads;
	};
}

#endif // BENCH_BENCHMARK_H
