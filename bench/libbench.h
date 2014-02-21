/*
 * libbench.h
 *
 *  Created on: 2013-09-22
 *      Author: creak
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
