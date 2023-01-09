cmake_minimum_required(VERSION 3.24)

include(FetchContent)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

message("${SRC_FILES}")

add_library(CppBenchmarkLib STATIC ${SRC_FILES})

target_include_directories(CppBenchmarkLib 
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/include/)

FetchContent_Declare(expected
        GIT_REPOSITORY git@github.com:TartanLlama/expected.git
        GIT_TAG v1.0.0)
    
FetchContent_MakeAvailable(expected)
target_link_libraries(CppBenchmarkLib expected)