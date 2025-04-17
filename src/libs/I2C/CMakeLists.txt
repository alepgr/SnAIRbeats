#CMake was not provided when library was cloned
#Quick file creating a STATIC library from it
add_library(I2C STATIC icm20948_i2c.cpp icm20948_utils.cpp)

#Uses links the include file for access to headers and definitions
target_include_directories(I2C PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)