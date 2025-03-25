add_subdirectory(libs/GPIO) 
add_subdirectory(libs/I2C)

find_package(PkgConfig REQUIRED)    

pkg_check_modules(MRAA REQUIRED mraa)
include_directories(${MRAA_INCLUDE_DIRS})

pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
include_directories(${LIBGPIOD_INCLUDE_DIRS})

find_package(yaml-cpp REQUIRED)


add_executable(SnairBeat main.cpp)

target_link_libraries(SnairBeat PRIVATE 
    GPIO 
    I2C 
    ${MRAA_LIBRARIES} 
    ${LIBGPIOD_LIBRARIES} 
    yaml-cpp)