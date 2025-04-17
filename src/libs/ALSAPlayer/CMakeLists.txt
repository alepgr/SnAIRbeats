#Probably unnecessary but find ALSA again
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALSA REQUIRED alsa)

#Create a STATIC library using ALSAPlayer.cpp
add_library(ALSAPlayer STATIC ALSAPlayer.cpp)

#Use the include directory for ALSAPlayer and ALSA
target_include_directories(ALSAPlayer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ALSA_INCLUDE_DIRS}

)

#Link required libraries together, ALSA in this case
target_link_libraries(ALSAPlayer PUBLIC ${ALSA_LIBRARIES})