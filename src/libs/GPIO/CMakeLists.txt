add_library(GPIO STATIC gpioevent.cpp)

target_include_directories(GPIO PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/libs/I2C/include
    ${CMAKE_SOURCE_DIR}/src/libs/IMUMaths/include
)

target_link_libraries(GPIO PRIVATE I2C IMUMATHS)
