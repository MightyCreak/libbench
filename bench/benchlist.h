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

#ifndef BENCH_H_
#define BENCH_H_

#include <time.h>
#include <vector>
#include <map>
#include <unordered_map>

namespace libbench
{
	struct Bench
	{
		uint32_t m_nameId;
		int m_parent;
		timespec m_start;
		timespec m_stop;
	};

	typedef std::vector<Bench> BenchVector;
	typedef std::unordered_map<uint32_t, char *> StringMap;

	class BenchList
	{
		friend class XmlWriter;
		friend class XmlReader;

	public:
		BenchList();

        BenchVector const& GetBenches() const;
        char const* GetBenchName(uint32_t id) const;
		void StartBench(char const* benchName);
		void StopBench();
		void Finalize();
		void Clear();

	private:
		Bench& AddBench(char const* benchName);

	private:
		static int const kNoParent;

		BenchVector m_benches;
		StringMap m_benchNames;
		int m_curNode;
	};
}

#endif /* BENCH_H_ */
