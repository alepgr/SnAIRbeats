#Turn IMU maths into a STATIC library
add_library(IMUMATHS STATIC IMUMaths.cpp)

#IMU Maths requires its include folder and the ALSAPlayer library
target_include_directories(IMUMATHS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/ALSAPlayer/include

)

#Link desired libraris - ALSAPlayer is defined in its respective CMAke found in the defined source
target_link_libraries(IMUMATHS PRIVATE )