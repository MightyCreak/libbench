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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <sched.h>
#include <glibmm/ustring.h>
#include "bench/bench.h"
#include "bench/helpers.h"
#include "bench/xmlcommon.h"
#include "bench/xmlreader.h"
#include "bench/xmlwriter.h"

void my_thread_process1()
{
    BENCH_SET_THREAD_NAME("process1");

    BENCH_SCOPED_PROFILE(my_thread_process1);
    for (int i = 0; i < 5; ++i)
    {
        BENCH_SCOPED_PROFILE(my_second_bench);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        BENCH_START_PROFILE(wait_500ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        BENCH_STOP_PROFILE();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void my_thread_process2(int startDelay)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(startDelay));

    BENCH_SET_THREAD_NAME("process2");

    BENCH_SCOPED_PROFILE(my_thread_process2);
    for (int i = 0; i < 50; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        BENCH_SCOPED_PROFILE(wait_20ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

int main(int argc, char ** argv)
{
    bench::Initialize();
    std::cout << "libbench v" << bench::GetVersionString() << std::endl;

    std::vector<std::thread> threads;

    // Call my_thread_process1 on core 0.
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        threads.push_back(std::thread(my_thread_process1));
        pthread_setaffinity_np(threads.back().native_handle(), sizeof(cpu_set_t), &cpuset);
    }

    // Call my_thread_process1 on core 1.
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(1, &cpuset);
        threads.push_back(std::thread(my_thread_process1));
        pthread_setaffinity_np(threads.back().native_handle(), sizeof(cpu_set_t), &cpuset);
    }

    // Call my_thread_process2 20 times randomly accross cores.
    unsigned int numCores = std::thread::hardware_concurrency();
    std::default_random_engine generator;
    std::uniform_int_distribution<int> coreDistrib(0, numCores - 1);
    std::uniform_int_distribution<int> waitDistrib(500, 1000);
    for(unsigned int u = 0; u < 20; ++u)
    {
        int coreId = coreDistrib(generator);
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(coreId, &cpuset);
        threads.push_back(std::thread(my_thread_process2, waitDistrib(generator)));
        pthread_setaffinity_np(threads.back().native_handle(), sizeof(cpu_set_t), &cpuset);
    }

    // Join all the threads.
    for(std::thread& thr : threads)
    {
        thr.join();
    }

    if(argc > 1)
    {
        std::ofstream filestream(argv[1], std::ofstream::out);
        if(!filestream)
            return EXIT_FAILURE;
        bench::WriteToXml(filestream);

        bench::XmlReader xmlReader;
        bench::Document doc;
        xmlReader.Read(Glib::ustring(argv[1]), doc);
    }
    else
    {
        bench::WriteToXml(std::cout);
    }

    bench::Shutdown();
    return EXIT_SUCCESS;
}
