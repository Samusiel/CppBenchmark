cmake_minimum_required(VERSION 3.24)

# set the project name
project(CppBenchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++20 -Werror")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fcoroutines-ts")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ -fcoroutines")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
endif()

list(APPEND SRC_FILES "${PROJECT_SOURCE_DIR}/main.cpp")

add_subdirectory(${PROJECT_SOURCE_DIR}/src/)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/tests/)

# add the executable
add_executable(CppBenchmark ${SRC_FILES})

target_link_libraries(CppBenchmark CppBenchmarkLib)