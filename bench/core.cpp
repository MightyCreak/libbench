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

#include "core.h"
#include "thread.h"

namespace
{
    void GetThreadName(pthread_t threadId, char * name, size_t maxsize)
    {
        pthread_getname_np(threadId, name, maxsize);
    }
}

namespace bench
{
    Core::~Core()
    {
        for(Thread* thread : m_threads)
        {
            delete thread;
        }
        m_threads.clear();
    }

    void Core::SetName(char const* name)
    {
        m_name = name;
    }

    char const* Core::GetName() const
    {
        return m_name.c_str();
    }

    Thread* Core::AddThread(pthread_t threadId)
    {
        char name[500];
        GetThreadName(threadId, name, 500);
        Thread* thread = new Thread(name);
        m_threads.push_back(thread);
        return thread;
    }
}
