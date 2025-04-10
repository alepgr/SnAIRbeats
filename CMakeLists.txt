cmake_minimum_required(VERSION 3.10)

project(SnairBeat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place built executables at top-level build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add the Qt5 folder globally
#include_directories(/usr/include/aarch64-linux-gnu/qt5)

add_subdirectory(src)
#add_subdirectory(tests)
