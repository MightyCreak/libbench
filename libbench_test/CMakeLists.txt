cmake_minimum_required(VERSION 2.8)
project(libbench)

# Add the executable.
add_executable(libbench_test
    main.cpp)

# C++ flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")

# Defines.
add_definitions(-DUSE_LIBBENCH)

# Add libraries.
set(LIBBENCH_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/..")
set(LIBBENCH_LIBRARIES "${PROJECT_SOURCE_DIR}/../libbench/release/libbench.so")
include_directories(${LIBBENCH_INCLUDE_DIRS})
target_link_libraries(libbench_test ${LIBBENCH_LIBRARIES})

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
endif (PKG_CONFIG_FOUND)

if (GTKMM_FOUND)
    include_directories(${GTKMM_INCLUDE_DIRS})
    target_link_libraries(libbench_test ${GTKMM_LIBRARIES})
endif (GTKMM_FOUND)

# Add "tags" target and make my_project depending on this target.
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_target(tags
    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_dependencies(libbench_test tags)

