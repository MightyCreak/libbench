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
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <sched.h>
#include <glibmm/ustring.h>
#include "bench/bench.h"
#include "bench/helpers.h"
#include "bench/xmlreader.h"
#include "bench/xmlwriter.h"

void * my_thread_process1(void * arg)
{
    pthread_setname_np(pthread_self(), "process1");

    LIBBENCH_SCOPED_PROFILE(my_thread_process2);
    for (int i = 0; i < 5; ++i)
    {
        LIBBENCH_SCOPED_PROFILE(my_second_bench);
        for (int j = 0; j < 1000000; ++j)
        {
            LIBBENCH_SCOPED_PROFILE(useless);
            j += 1;
            j -= 1;
        }
        i += 1;
        i -= 1;
    }

    pthread_exit(0);
}

void * my_thread_process2(void * arg)
{
    pthread_setname_np(pthread_self(), "process2");

    LIBBENCH_SCOPED_PROFILE(my_thread_process1);
    for (int i = 0; i < 5; ++i)
    {
        LIBBENCH_SCOPED_PROFILE(my_second_bench);
        for (int j = 0; j < 1000000; ++j)
        {
            j += 1;
            j -= 1;
        }
        i += 1;
        i -= 1;
    }

    pthread_exit(0);
}

void * my_thread_process3(void * arg)
{
    pthread_setname_np(pthread_self(), "process3");

    LIBBENCH_SCOPED_PROFILE(my_thread_process3);
    for (int i = 0; i < 10000; ++i)
    {
        LIBBENCH_SCOPED_PROFILE(useless);
        i += 1;
        i -= 1;
    }

    pthread_exit(0);
}

int main(int argc, char ** argv)
{
    bench::Manager::CreateInstance();

    unsigned int thrIdx = 0;
    pthread_t thr[20];
    pthread_attr_t thr_attr;
    cpu_set_t cpuset;

    pthread_attr_init(&thr_attr);
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_attr_setaffinity_np(&thr_attr, sizeof(cpu_set_t), &cpuset);
    if (pthread_create(&thr[thrIdx], &thr_attr, my_thread_process1, nullptr) == 0) {
        ++thrIdx;
    }
    else {
        std::cerr << "pthread_create error for thread " << thrIdx << std::endl;
    }
    pthread_attr_destroy(&thr_attr);

    pthread_attr_init(&thr_attr);
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    pthread_attr_setaffinity_np(&thr_attr, sizeof(cpu_set_t), &cpuset);
    if (pthread_create(&thr[thrIdx], &thr_attr, my_thread_process2, nullptr) == 0) {
        ++thrIdx;
    }
    else {
        std::cerr << "pthread_create error for thread " << thrIdx << std::endl;
    }
    pthread_attr_destroy(&thr_attr);

    unsigned int numCores = std::thread::hardware_concurrency();
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, numCores - 1);
    while(thrIdx < 20)
    {
        int coreId = distribution(generator);

        pthread_attr_init(&thr_attr);
        CPU_ZERO(&cpuset);
        CPU_SET(coreId, &cpuset);
        pthread_attr_setaffinity_np(&thr_attr, sizeof(cpu_set_t), &cpuset);
        if (pthread_create(&thr[thrIdx], &thr_attr, my_thread_process3, nullptr) == 0) {
            ++thrIdx;
        }
        else {
            std::cerr << "pthread_create error for thread " << thrIdx << std::endl;
        }
        pthread_attr_destroy(&thr_attr);
    }

    for(unsigned int u = 0; u < thrIdx; ++u)
    {
        void * ret;
        pthread_join(thr[u], &ret);
    }

    bench::Manager * lbManager = bench::Manager::GetInstance();
    lbManager->Finalize();
    if(argc > 1)
    {
        std::ofstream filestream(argv[1], std::ofstream::out);
        if(!filestream)
            return EXIT_FAILURE;
        lbManager->Write(filestream);

//        libbench::XmlReader xmlReader;
//        libbench::BenchMark readBenchmark;
//        xmlReader.Read(Glib::ustring(argv[1]), readBenchmark);

//        std::ofstream testfilestream("test.xml", std::ofstream::out);
//        libbench::XmlWriter xmlWriter(testfilestream);
//        xmlWriter.Write(readBenchmark);
    }
    else
    {
        lbManager->Write(std::cout);
    }

    bench::Manager::DestroyInstance();
    return EXIT_SUCCESS;
}
