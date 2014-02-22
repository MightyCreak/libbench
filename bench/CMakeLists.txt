cmake_minimum_required(VERSION 2.8)
project(libbench)

# The version number.
set(libbench_VERSION_MAJOR 0)
set(libbench_VERSION_MINOR 1)

# Add the library.
add_library(bench SHARED
    benchlist.h
    benchlist.cpp
    benchmark.h
    benchmark.cpp
    bench.h
    bench.cpp
    helpers.h
    manager.h
    manager.cpp
    thread.h
    thread.cpp
    xmlcommon.h
    xmlreader.h
    xmlreader.cpp
    xmlwriter.h
    xmlwriter.cpp)

# Configure a header file to pass some of the CMake settings
# to the source code.
configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h")

# Add the binary tree to the search path for include files
# so that we will find config.h.
include_directories(${PROJECT_BINARY_DIR})

# C++ flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Add libraries.
find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(LIBXMLPP REQUIRED libxml++-2.6)
endif (PKG_CONFIG_FOUND)

if (LIBXMLPP_FOUND)
    include_directories(${LIBXMLPP_INCLUDE_DIRS})
    target_link_libraries(bench ${LIBXMLPP_LIBRARIES})
endif (LIBXMLPP_FOUND)

# Add "tags" target and make my_project depending on this target.
#set_source_files_properties(tags PROPERTIES GENERATED true)
#add_custom_target(tags
#    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#add_dependencies(bench tags)
