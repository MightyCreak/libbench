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
