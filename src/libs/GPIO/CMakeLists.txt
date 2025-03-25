add_library(GPIO STATIC gpioevent.cpp)

target_include_directories(GPIO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)