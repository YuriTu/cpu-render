cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(VulkanRender VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vulkan_render main.cpp)