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

#include "bench.h"
#include "manager.h"
#include "config.h"

namespace bench
{
    char const* GetVersionString()
    {
        return BENCH_VERSION;
    }

    void Initialize()
    {
        Manager::CreateInstance();
    }

    void Shutdown()
    {
        Manager::DestroyInstance();
    }

    void SetCoreName(unsigned int core, char const* name)
    {
        Manager::GetInstance()->SetCoreName(core, name);
    }

    void SetThreadName(char const* name)
    {
        Manager::GetInstance()->SetThreadName(name);
    }

    void StartBench(char const* name)
    {
        Manager::GetInstance()->StartBench(name);
    }

    void StopBench()
    {
        Manager::GetInstance()->StopBench();
    }

    void WriteToXml(std::ostream& stream)
    {
        Manager::GetInstance()->Write(stream);
    }
}
