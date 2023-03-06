cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(CPU_RENDER VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(cpu_render src/main.cpp)

include_directories(src/core src/template)

FILE ( GLOB CORE_SOURCE
    src/core/*
)

FILE ( GLOB TEMP_SOURCE
    src/template/*
)

# if(MSVC)
#   add_compile_options(/W4 /WX)
#   add_compile_options(/wd4819)  # 关闭“文件包含一个当前代码页（936）不能表示的字符”警告
# else()
#   add_compile_options(-Wall -Wextra -pedantic -Werror)
#   add_compile_options(-Wno-invalid-source-encoding)  # 关闭“文件包含一个当前代码页（936）不能表示的字符”警告
# endif()

add_library(CRLibrary STATIC ${CORE_SOURCE} ${TEMP_SOURCE})
target_link_libraries(cpu_render CRLibrary)

