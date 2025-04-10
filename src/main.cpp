#include "libs/I2C/include/icm20948_i2c.hpp"
#include "libs/I2C/include/icm20948_defs.hpp"

#include "libs/GPIO/include/gpioevent.h"

#include "libs/ALSAAudio/include/AudioLib.hpp"

#include "libs/IMUMaths/include/IMUMaths.hpp"

#include "libs/PlayAudio/include/PlayAudio.hpp"

#include <iostream> 
#include <iomanip>
#include <gpiod.h>

#include <chrono>
#include <thread>

#include <bitset>
#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 17

//Object for I2C operations
icm20948::ICM20948_I2C objI2C(1); // bus number 1 means it is communicating with an external device

//Object for Playing audio
PlayAudioName::PlayAudio objAudio;

//Object for Maths operations, passing values in
IMUMathsName::IMUMaths objMaths(&objAudio);

//Object for GPIO operations
GPIOName::GPIOClass objGPIO("gpiochip0", 17, 27, objI2C, objMaths);

int main()
{
    objI2C.settings.accel.sample_rate_div = 0;
    objI2C.settings.gyro.sample_rate_div = 0;
    objI2C.settings.accel.scale = icm20948::ACCEL_16G;
    objI2C.settings.gyro.scale = icm20948::GYRO_2000DPS;
    
    std::cout << "Object created!\n";
    if(objI2C.init())
    {
        std::cout << "Hurray!" << std::endl;

        if (objI2C.enable_DRDY_INT()){
            std::cout << "WOM Interrupt Enabled" << std::endl;
        } else {
            std::cerr <<"Failed to enable WOM interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky" << std::endl;
        return -1;
    }


    std::thread gpioThread(&GPIOName::GPIOClass::Worker, &objGPIO);

    //objGPIO.running = false;

    // Join the thread to clean up properly.
    if (gpioThread.joinable())
        gpioThread.join();

    std::cout << "Exiting program" << std::endl;
    return 0;

}
