add_subdirectory(libs/GPIO) 
add_subdirectory(libs/I2C)
add_subdirectory(libs/cam2opencv)
#add_subdirectory(libs/DataCollection)
add_subdirectory(libs/ALSAAudio)

find_package(PkgConfig REQUIRED)

# pkg_check_modules(QWT REQUIRED qwt)
# include_directories(${QWT_INCLUDE_DIRS})

find_package(Qt5 5.9 COMPONENTS Core Widgets REQUIRED)

pkg_check_modules(MRAA REQUIRED mraa)
include_directories(${MRAA_INCLUDE_DIRS})

pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
include_directories(${LIBGPIOD_INCLUDE_DIRS})

pkg_check_modules(ALSA REQUIRED alsa)
include_directories(${ALSA_INCLUDE_DIRS})


find_package(yaml-cpp REQUIRED)


add_executable(SnairBeat main.cpp)

target_link_libraries(SnairBeat PRIVATE 
    GPIO 
    I2C 
    #DataCollection
    cam2opencv
    ALSAAudio
    ${MRAA_LIBRARIES} 
    ${LIBGPIOD_LIBRARIES} 
    ${ALSA_LIBRARIES}
    yaml-cpp
    Qt5::Widgets)

    