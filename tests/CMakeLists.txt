find_package(GTest REQUIRED)

add_executable(run_tests
MathsTests.cpp
 )


 enable_testing()

 target_link_libraries(run_tests PRIVATE 
 GPIO 
 I2C 
 PLAYAUDIO
 ${MRAA_LIBRARIES} 
 ${LIBGPIOD_LIBRARIES} 
 ${ALSA_LIBRARIES}
 yaml-cpp
 IMUMATHS
 ALSAPlayer
 gtest     
 gtest_main 
)



add_test(NAME Test_all COMMAND ${CMAKE_BINARY_DIR}/run_tests)


    