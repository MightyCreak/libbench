/*
 * benchmark.h
 *
 *  Created on: 2013-10-12
 *      Author: creak
 */

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <vector>
#include <cstddef>

namespace libbench
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

#endif /* BENCHMARK_H_ */
