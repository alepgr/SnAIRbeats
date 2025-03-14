// Custom headers to define communication with the IMU sensor
#include "icm20948_i2c.hpp"
#include "icm20948_defs.hpp"

#include "gpioevent.h"

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

//Global pointer so the int handler can access sensor instance
icm20948::ICM20948_I2C* g_imu = nullptr;
gpiod_chip *chip;
gpiod_line *line;

int GPIO(){
    // Open GPIO chip
    gpiod_chip *chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip: " << GPIO_CHIP << std::endl;
        return 1;
    }

    // Open the specific GPIO line
    gpiod_line *line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (!line) {
        std::cerr << "Failed to get GPIO line " << GPIO_LINE << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Request the line as an input
    if (gpiod_line_request_input(line, "gpio_reader") < 0) {
        std::cerr << "Failed to set GPIO line as input." << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Read GPIO value
    int value = gpiod_line_get_value(line);
    if (value < 0) {
        std::cerr << "Failed to read GPIO value." << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Determine active state
    if (value == 1) {
        std::cout << "GPIO 17 is HIGH (Active High if configured that way)" << std::endl;
    } else {
        std::cout << "GPIO 17 is LOW (Active Low if configured that way)" << std::endl;
    }

    // Cleanup
    gpiod_chip_close(chip);
    return 0;
}




int main()
{
    // Creating an instance of the class, inside the namespace
    icm20948::ICM20948_I2C obj(1); // bus number 1 means it is communicating with an external device

    g_imu = &obj;

    std::cout << "Object created!\n";
    if(obj.init())
    {
        std::cout << "Hurray!" << std::endl;

        if (obj.enable_wom_interrupt(0xFF)){
            std::cout << "WOM Interrupt Enabled" << std::endl;
        } else {
            std::cerr <<"Failed to enable WOM interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky" << std::endl;
        return -1;
    }


    while (true) {

        GPIO();
        //Wake up if motion
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (obj.check_wom_interrupt()) {
             std::cout << "Motion detected!!" <<std::endl;

        //     if (!obj.read_accel_gyro()) {
        //         std::cout << "read_accel_gyro() failed!\nWomp Womp" << std::endl;
        //         break;
        //     }

        //     std::cout << std::setprecision(2) << "Accel:\n"
        //                                      "  x = " << obj.accel[0] << std::endl <<
        //                                      "  y = " << obj.accel[1] << std::endl <<
        //                                      "  z = " << obj.accel[2] << std::endl << std::endl;
        }
    }

    
}

