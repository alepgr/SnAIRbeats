// Custom headers to define communication with the IMU sensor
#include "libs/I2C/include/icm20948_i2c.hpp"
#include "libs/I2C/include/icm20948_defs.hpp"

#include "libs/GPIO/include/gpioevent.h"

#include <iostream> // "input output" stream, to print messages
#include <iomanip> // to "manipulate"/format the output messages
#include <gpiod.h>

// Timing operations
#include <chrono>
#include <thread>

// For printing binary values (used in commented out debugging code)
#include <bitset>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 17


int main()
{
    // Creating an instance of the class, inside the namespace
    icm20948::ICM20948_I2C obj(1); // bus number 1 means it is communicating with an external device

    // g_imu = &obj;

    std::cout << "Object created!\n";
    if(obj.init())
    {
        std::cout << "Hurray!" << std::endl;

        if (obj.enable_wom_interrupt(0x0F)){
            std::cout << "WOM Interrupt Enabled" << std::endl;
        } else {
            std::cerr <<"Failed to enable WOM interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky" << std::endl;
        return -1;
    }



    obj.Worker();
    
}

