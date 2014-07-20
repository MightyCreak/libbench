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

#ifndef BENCH_BENCH_H
#define BENCH_BENCH_H

#include <ostream>

extern "C"
{

namespace bench
{
    /// Get the version string.
    /// @return Version string.
    char const* GetVersionString();

    /// Initialize the library.
    void Initialize();

    /// Shutdown the library.
    void Shutdown();

    /// Set the name of a core.
    /// @param core Core index.
    /// @param name New name of the core.
    void SetCoreName(unsigned int core, char const* name);

    /// Set the name of the current thread.
    /// @param name New name of the thread.
    void SetThreadName(char const* name);

    /// Start a new bench.
    /// @param name Name of the bench.
    void StartBench(char const* name);

    /// Stop the current bench.
    void StopBench();

    /// Write the profiling in an XML file.
    void WriteToXml(std::ostream& stream);
}

}

#endif // BENCH_BENCH_H
