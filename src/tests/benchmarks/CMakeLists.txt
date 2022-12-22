cmake_minimum_required(VERSION 3.24)

file(GLOB_RECURSE BENCHMARK_FILES *.cpp)

FetchContent_Declare(GoogleBenchmark
    GIT_REPOSITORY git@github.com:google/benchmark.git
    GIT_TAG v1.7.1)

FetchContent_MakeAvailable(GoogleBenchmark)

foreach(FileToBenchmark ${BENCHMARK_FILES})
    cmake_path(GET FileToBenchmark STEM BenchmarkName)
    # add the executable
    add_executable(${BenchmarkName} ${FileToBenchmark})
    target_link_libraries(${BenchmarkName} benchmark::benchmark CppBenchmarkLib)
endforeach()