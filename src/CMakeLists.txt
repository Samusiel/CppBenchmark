cmake_minimum_required(VERSION 3.24)

file(GLOB_RECURSE SRC_FILES "src/*.[h|c]pp")

message("${SRC_FILES}")

add_library(CppBenchmarkLib STATIC ${SRC_FILES})

target_include_directories(CppBenchmarkLib 
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include/)