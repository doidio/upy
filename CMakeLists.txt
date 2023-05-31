cmake_minimum_required(VERSION 3.15...3.22)
project(upy LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Python3 3.10 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

add_library(upy cpp/upy.cpp)
target_link_libraries(upy PRIVATE pybind11::embed)

add_executable(test cpp/test.cpp)
target_link_libraries(test PRIVATE upy pybind11::embed)
