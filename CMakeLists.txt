cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(ising LANGUAGES C CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 23)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

find_package(imgui QUIET)

if (NOT TARGET imgui::imgui)
  message(STATUS "--- IMGUI not found on system. Fetching content.")
  FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.0-docking
  )
  FetchContent_MakeAvailable(imgui)
  set(IMGUI_SRC "${CMAKE_BINARY_DIR}/_deps/imgui-src")
endif()

find_package(implot QUIET)

if (NOT TARGET implot::implot)
  message(STATUS "--- IMPLOT not found on system. Fetching content")
  FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot
    GIT_TAG 3da8bd34299965d3b0ab124df743fe3e076fa222 #v0.16
  )
  FetchContent_MakeAvailable(implot)
  set(IMPLOT_SRC "${CMAKE_BINARY_DIR}/_deps/implot-src")
endif()

add_subdirectory(external)

add_subdirectory(src)
