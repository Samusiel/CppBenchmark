cmake_minimum_required(VERSION 3.24)

file(GLOB_RECURSE TEST_FILES *.cpp)

add_executable(CppBenchmarkTests ${TEST_FILES})

target_link_libraries(CppBenchmarkTests GTest::gtest_main CppBenchmarkLib)

include(GoogleTest)
gtest_discover_tests(CppBenchmarkTests)

