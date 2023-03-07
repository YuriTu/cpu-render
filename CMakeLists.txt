cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(CPU_RENDER VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(cpu_render src/main.cpp)

include_directories(src/core src/template src/lib)

FILE ( GLOB CORE_SOURCE
    src/core/*
)

FILE ( GLOB TEMP_SOURCE
    src/template/*
)

FILE ( GLOB LIB_SOURCE
    src/lib/*
)


add_library(CRLibrary STATIC ${CORE_SOURCE} ${TEMP_SOURCE} ${LIB_SOURCE})
target_link_libraries(cpu_render CRLibrary)
# target_link_libraries(cpu_render)

