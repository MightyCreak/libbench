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

#ifndef BENCH_THREAD_H
#define BENCH_THREAD_H

#include <vector>
#include <unordered_map>

namespace bench
{
    class Thread
    {
    public:
        friend class Manager;

        Thread();
        ~Thread();

        void SetName(char const* name);
        char const* GetName() const;
        char const* GetBenchName(uint32_t id) const;
        void StartBench(char const* name);
        void StopBench();
        void Finalize();
        void Clear();

    private:
        struct Bench
        {
            uint32_t m_nameId;
            int m_parent;
            timespec m_start;
            timespec m_stop;
        };

        typedef std::vector<Bench> BenchVector;
        typedef std::unordered_map<uint32_t, char *> StringMap;

    private:
        Bench& AddBench(char const* name);

    private:
        static int const kNoParent;

        char* m_name;
        BenchVector m_benches;
        StringMap m_benchNames;
        int m_curNode;
    };
}

#endif // BENCH_THREAD_H
