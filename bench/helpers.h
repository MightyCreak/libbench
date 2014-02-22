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

#ifndef BENCH_HELPERS_H
#define BENCH_HELPERS_H

#ifdef USE_LIBBENCH

namespace bench
{
    class ScopedBench
    {
    public:
        ScopedBench(char const* name)
        {
            StartBench(name);
        }

        ~ScopedBench()
        {
            StopBench();
        }
    };
}

#	define LIBBENCH_START_PROFILE(name)		bench::StartBench(#name)
#	define LIBBENCH_STOP_PROFILE()			bench::StopBench()
#	define LIBBENCH_SCOPED_PROFILE(name)	bench::ScopedBench __libbench ## name(#name)

#else

#	define LIBBENCH_START_PROFILE(name)
#	define LIBBENCH_STOP_PROFILE()
#	define LIBBENCH_SCOPED_PROFILE(name)

#endif

#endif // BENCH_HELPERS_H
