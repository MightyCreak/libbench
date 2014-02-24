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

#ifndef BENCH_MANAGER_H
#define BENCH_MANAGER_H

#include <vector>
#include <map>
#include <ostream>
#include "xmlcommon.h"

namespace bench
{
    class Core;
    class Thread;

    class Manager
    {
    public:
        friend class XmlWriter;
        friend class XmlReader;

        static void CreateInstance();
        static void DestroyInstance();
        static Manager* GetInstance();

        ~Manager();

        void SetCoreName(unsigned int core, char const* name);
        void StartBench(char const* name);
        void StopBench();
        void Clear();
        void Write(std::ostream& stream) const;

    private:
        Manager();

        void ConvertBenchChildren(Thread const& thread,
                                  int parentIdx,
                                  std::vector<DocumentBench>& docBenches,
                                  __time_t starttime) const;
        Thread* RegisterThread(pthread_t thread);

    private:
        static Manager* ms_instance;

        typedef std::vector<Core*> CoreVector;
        typedef std::map<pthread_t, Thread*> IdThreadMap;

        CoreVector m_cores;
        IdThreadMap m_threads;
        mutable pthread_mutex_t m_mutex;
    };
}

#endif // BENCH_MANAGER_H
