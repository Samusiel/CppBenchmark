cmake_minimum_required(VERSION 3.24)

include(FetchContent)

FetchContent_Declare(GoogleTest
    GIT_REPOSITORY git@github.com:google/googletest.git
    GIT_TAG v1.12.0)

FetchContent_MakeAvailable(GoogleTest)

add_subdirectory(benchmarks)
add_subdirectory(gtests)