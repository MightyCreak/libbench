/*
 * bench.h
 *
 *  Created on: 2013-09-25
 *      Author: creak
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
