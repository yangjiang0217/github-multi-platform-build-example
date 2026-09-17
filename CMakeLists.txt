cmake_minimum_required(VERSION 3.20)
project(hello_world C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_executable(hello hello.c)

# Linux 下静态链接，产物可直接分发，不依赖目标机器上的动态库
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(hello PRIVATE -static)
endif()
