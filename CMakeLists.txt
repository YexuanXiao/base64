cmake_minimum_required(VERSION 3.29)

project(base64)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(benchmark benchmark.cpp)
target_compile_options(benchmark PRIVATE -fno-exceptions -fno-rtti)
add_executable(examples examples.cpp)
target_compile_options(examples PRIVATE -fno-exceptions -fno-rtti)