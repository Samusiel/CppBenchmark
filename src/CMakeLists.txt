cmake_minimum_required(VERSION 3.24)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_library(CppBenchmarkLib ${SRC_FILES})

target_include_directories(CppBenchmarkLib 
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include/)