find_package(PkgConfig REQUIRED)
pkg_check_modules(ALSA REQUIRED alsa)


add_library(ALSAPlayer STATIC ALSAPlayer.cpp)

target_include_directories(ALSAPlayer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ALSA_INCLUDE_DIRS}

)

target_link_libraries(ALSAPlayer PUBLIC ${ALSA_LIBRARIES})