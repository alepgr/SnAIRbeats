find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

add_executable(run_tests
 tests/test_main.cpp)

target_link_libraries(SnairBeat PRIVATE 
GTest::GTest
GMock::GMock)

enable_testing()

add_test(NAME TestMain COMMAND test_main)

    