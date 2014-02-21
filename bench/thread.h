/*
 * thread.h
 *
 *  Created on: 2013-09-25
 *      Author: creak
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <vector>
#include "benchlist.h"

namespace libbench
{
	class Thread
	{
		friend class XmlWriter;
		friend class XmlReader;

	public:
		Thread(char const* name, int coreId);
		~Thread();

        char const* GetName() const;
        int GetCoreId() const;
        BenchList const& GetBenchList() const;
		void StartBench(char const* benchName);
		void StopBench();
		void Finalize();
		void Clear();

	private:
		char* m_name;
		int m_coreId;
		BenchList m_benchList;
	};
}

#endif /* THREAD_H_ */
