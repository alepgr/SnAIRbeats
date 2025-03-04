#include <mraa.hpp>
#include <iostream>

int main() {
    mraa_init();
    std::cout << "MRAA Version: " << mraa_get_version() << std::endl;
    return 0;
}
