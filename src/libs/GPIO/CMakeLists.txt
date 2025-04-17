#Create a STATIC library called GPIO from the gpioevent.cpp
add_library(GPIO STATIC gpioevent.cpp)

#GPIO requires files inside its own include folder
#and the I2C drivers for the IMU and the IMUMaths library
target_include_directories(GPIO PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/libs/I2C/include
    ${CMAKE_SOURCE_DIR}/src/libs/IMUMaths/include
)

#Link the required libraries. I2C and IMUMaths names are defined
#in their respective CMakeLists.txt
target_link_libraries(GPIO PRIVATE I2C IMUMATHS)
