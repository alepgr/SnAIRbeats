find_package(PkgConfig REQUIRED)
pkg_check_modules(ALSA Required alsa)

add_library(PLAYAUDIO STATIC PlayAudio.cpp)

target_include_directories(PLAYAUDIO PUBLIC ${ALSA_INCLUDE_DIRS})

target_link_libraries(PLAYAUDIO PUBLIC ${ALSA_LIBRARIES})