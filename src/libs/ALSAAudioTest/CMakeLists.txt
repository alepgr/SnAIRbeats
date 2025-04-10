cmake_minimum_required(VERSION 3.10)
project(ALSAAudio LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALSA REQUIRED alsa)

add_library(ALSAAudio STATIC AudioLib.cpp)

target_include_directories(ALSAAudio PUBLIC ${ALSA_INCLUDE_DIRS})

target_link_libraries(ALSAAudio PUBLIC ${ALSA_LIBRARIES})