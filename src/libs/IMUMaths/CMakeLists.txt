add_library(IMUMATHS STATIC IMUMaths.cpp)

target_include_directories(IMUMATHS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/PlayAudio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/ALSAPlayer/include

)

target_link_libraries(IMUMATHS PRIVATE PLAYAUDIO ALSAPlayer)