add_library(I2C STATIC icm20948_i2c.cpp icm20948_utils.cpp)

target_include_directories(I2C PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)