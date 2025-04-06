add_library(IMUMATHS STATIC IMUMaths.cpp)

target_include_directories(IMUMATHS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)

target_link_libraries(IMUMATHS PRIVATE)