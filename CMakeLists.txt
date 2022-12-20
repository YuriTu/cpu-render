cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(CpuRender VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpu_render main.cpp)