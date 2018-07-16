cmake_minimum_required (VERSION 3.6)
project (ProgParallel CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# CMake generators if no build type is set.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

find_package(Threads REQUIRED)
find_package(benchmark REQUIRED)
find_package(GTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -fsanitize=thread")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(src/)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_SUFFIX)

message(STATUS "-------------------------------")
message(STATUS "++ Setup: ")
message(STATUS "System:       " ${CMAKE_SYSTEM})
message(STATUS "Build dir:    " ${CMAKE_BINARY_DIR})
message(STATUS "Build type:   " ${CMAKE_BUILD_TYPE})
message(STATUS "CXX:          " ${CMAKE_CXX_COMPILER_ID})
message(STATUS "CXXFLAGS:     " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE_SUFFIX}})
message(STATUS "Verbose:      " ${CMAKE_VERBOSE_MAKEFILE})
message(STATUS "")
message(STATUS "++ Lib Info")
message(STATUS "ar:           " ${CMAKE_AR})
message(STATUS "ranlib:       " ${CMAKE_RANLIB})
message(STATUS "-------------------------------")
