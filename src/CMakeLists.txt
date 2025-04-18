#CMake for main executable for the project

#Each subdirectory relates
add_subdirectory(libs/GPIO) 
add_subdirectory(libs/I2C)
add_subdirectory(libs/IMUMaths)
#add_subdirectory(libs/ALSAPlayer)

#Package finding module, helps to locate imported packages
find_package(PkgConfig REQUIRED)

#Check for installation of MRAA and include its directories
pkg_check_modules(MRAA REQUIRED mraa)
include_directories(${MRAA_INCLUDE_DIRS})

#Check for installation of lipgpiod and include its directories
pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
include_directories(${LIBGPIOD_INCLUDE_DIRS})

#Checl for installation of ALSA and include its directories
pkg_check_modules(ALSA REQUIRED alsa)
include_directories(${ALSA_INCLUDE_DIRS})

#Find YAML package
find_package(yaml-cpp REQUIRED)

#Create the executable for the project via main.cpp called "SnairBeat"
#Run using ./SnairBeat at the top level
add_executable(SnairBeat main.cpp)

#Link the required packages to the SnairBeat project
#Names are defined within their respective CMakeLists.txt
target_link_libraries(SnairBeat PRIVATE 
    GPIO 
    I2C 
    ${MRAA_LIBRARIES} 
    ${LIBGPIOD_LIBRARIES} 
    ${ALSA_LIBRARIES}
    yaml-cpp
    IMUMATHS)

    