add_library(DATACOLLECTION STATIC DataCollection.cpp)

target_include_directories(DATACOLLECTION PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/libs/I2C/include
)

target_link_libraries(DATACOLLECTION PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)