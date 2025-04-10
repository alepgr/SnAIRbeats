add_library(GPIO STATIC gpioevent.cpp)

find_package(OpenCV REQUIRED)

find_package(PkgConfig)
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

find_package(Qt5Widgets REQUIRED)

target_include_directories(GPIO PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/libs/I2C/include
    ${CMAKE_SOURCE_DIR}/src/libs/IMUMaths/include
    ${CMAKE_SOURCE_DUR}/src/libs/PlayAudio/include
    ${CMAKE_SOURCE_DIR}/src/libs/CameraMaths/include
    /usr/include/aarch64-linux-gnu/qt5/QtCore
    /usr/include/aarch64-linux-gnu/qt5
    ${Qt5Widgets_INCLUDE_DIRS}
)

target_link_libraries(GPIO PRIVATE I2C 
    IMUMATHS
    PkgConfig::LIBCAMERA 
    PLAYAUDIO 
    CAMERAMATHS 
    ${OpenCV_LIBS}
    /usr/lib/libqwt-qt5.so)
