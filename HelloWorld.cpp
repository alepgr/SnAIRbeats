#include <iostream>
#include <thread>  // For sleep functionality
#include <chrono>  // For time duration

int main() {
    while (true) {
        std::cout << "Hello World" << std::endl;  // Print "Hello World"
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Wait for 1 second
    }
    return 0;
}
