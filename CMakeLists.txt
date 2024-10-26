cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(ising LANGUAGES C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 23)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(external)

add_subdirectory(src)
