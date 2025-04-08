add_subdirectory(libs/GPIO) 
add_subdirectory(libs/I2C)
add_subdirectory(libs/cam2opencv)
#add_subdirectory(libs/DataCollection)
add_subdirectory(libs/PlayAudio)
add_subdirectory(libs/IMUMaths)

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

find_path(QWT_INCLUDE_DIR
  NAMES qwt/qwt_plot.h
  PATHS /usr/include
)

if (NOT QWT_INCLUDE_DIR)
  message(FATAL_ERROR "Qwt include directory not found!")
endif()

find_library(QWT_LIBRARY
  NAMES qwt-qt5 libqwt-qt5.so libqwt-qt5.so.6 libqwt-qt5.so.6.1 libqwt-qt5.so.6.1.4
  PATHS /usr/lib /usr/lib/arm-linux-gnueabihf /usr/lib/aarch64-linux-gnu
)

if (NOT QWT_LIBRARY)
  message(FATAL_ERROR "Qwt library not found!")
endif()

message(STATUS "Found Qwt library: ${QWT_LIBRARY}")

message(STATUS "Found Qwt include directory: ${QWT_INCLUDE_DIR}")
message(STATUS "Found Qwt library: ${QWT_LIBRARY}")

find_package(yaml-cpp REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(SnairBeat main.cpp libs/cam2opencv/qtviewer/window.cpp)

target_link_libraries(SnairBeat PRIVATE 
    GPIO 
    I2C 
    #DataCollection
    cam2opencv
    PLAYAUDIO
    ${MRAA_LIBRARIES} 
    ${LIBGPIOD_LIBRARIES} 
    ${ALSA_LIBRARIES}
    yaml-cpp
    Qt5::Widgets
    IMUMATHS
    ${QWT_LIBRARY})

    