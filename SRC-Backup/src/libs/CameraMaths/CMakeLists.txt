add_library(CAMERAMATHS STATIC CameraMaths.cpp cam2opencv/libcam2opencv.cpp cam2opencv/qtviewer/window.cpp)

add_subdirectory(cam2opencv)

find_package(Qt5Core REQUIRED)
message(STATUS "Qt5Core library found:")
message(STATUS "    version: ${Qt5Core_VERSION}")
message(STATUS "    include path: ${Qt5Core_INCLUDE_DIRS}")
message(STATUS "    libraries: ${Qt5Core_LIBRARIES}")

find_package(Qt5Widgets REQUIRED)

target_include_directories(CAMERAMATHS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/cam2opencv
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Qt5Core_INCLUDE_DIRS}
    /usr/include/aarch64-linux-gnu/qt5/QtCore
)

target_include_directories(CAMERAMATHS SYSTEM BEFORE PUBLIC
    /usr/include/aarch64-linux-gnu/qt5/QtCore
    
)

set_target_properties(CAMERAMATHS PROPERTIES AUTOMOC ON)

target_link_libraries(CAMERAMATHS PRIVATE cam2opencv
    Qt5::Widgets
    #PkgConfig::LIBCAMERA
    ${OpenCV_LIBS}
    /usr/lib/libqwt-qt5.so)