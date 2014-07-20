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

#ifndef BENCH_CORE_H
#define BENCH_CORE_H

#include <vector>
#include <string>
#include <thread>

namespace bench
{
    class Thread;

    class Core
    {
    public:
        friend class Manager;

        ~Core();

        void SetName(char const* name);
        char const* GetName() const;

        Thread* AddThread();

    private:
        typedef std::vector<Thread*> ThreadVector;

        std::string m_name;
        ThreadVector m_threads;
    };
}

#endif // BENCH_CORE_H
