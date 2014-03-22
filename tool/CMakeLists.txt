cmake_minimum_required(VERSION 2.8)
project(bench_viewer)

# Add the executable.
add_executable(libbench_gtk
    benchmarkarea.h
    benchmarkarea.cpp
    drawbench.h
    drawbench.cpp
    drawcore.h
    drawcore.cpp
    drawthread.h
    drawthread.cpp
    libbenchwindow.h
    libbenchwindow.cpp
    rectangle.h
    timeline.h
    timeline.cpp
    main.cpp)

# C++ flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -pthread") #-Werror

# Defines.
#add_definitions(-DUSE_LIBBENCH)

# Add libraries.
set(LIBBENCH_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/..")
set(LIBBENCH_LIBRARIES "${PROJECT_BINARY_DIR}/../bench/libbench.so")
include_directories(${LIBBENCH_INCLUDE_DIRS})
target_link_libraries(libbench_gtk ${LIBBENCH_LIBRARIES})

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
endif (PKG_CONFIG_FOUND)

if (GTKMM_FOUND)
    include_directories(${GTKMM_INCLUDE_DIRS})
    target_link_libraries(libbench_gtk ${GTKMM_LIBRARIES})
endif (GTKMM_FOUND)

# Add "tags" target and make my_project depending on this target.
#set_source_files_properties(tags PROPERTIES GENERATED true)
#add_custom_target(tags
#    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#add_dependencies(libbench_gtk tags)
