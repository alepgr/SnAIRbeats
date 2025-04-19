find_package(GTest REQUIRED)

add_executable(run_tests
GPIOTests.cpp
MathsTests.cpp
AudioTests.cpp
)


 enable_testing()

 target_link_libraries(run_tests PRIVATE 
 GPIO 
 I2C 
 ${MRAA_LIBRARIES} 
 ${LIBGPIOD_LIBRARIES} 
 ${ALSA_LIBRARIES}
 yaml-cpp
 IMUMATHS
 gtest     
 gtest_main 
 GTest::gmock
)

add_test(NAME Test_all COMMAND ${CMAKE_BINARY_DIR}/run_tests)

set_tests_properties(Test_all PROPERTIES
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

target_compile_definitions(run_tests PRIVATE UNIT_TEST)