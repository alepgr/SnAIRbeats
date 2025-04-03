#ifndef GPIOEVENT_H
#define GPIOEVENT_H


#include <gpiod.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <cstdint>

#include "../../I2C/include/icm20948_i2c.hpp"
#include "../../I2C/include/icm20948_utils.hpp"

namespace GPIOName {
    class GPIOClass {
    private:
        gpiod_chip* chip;
        gpiod_line* SensorLine;
        gpiod_line* LEDLine;
        int InterruptPin;
        int LEDPin;
        int Counter;
    
    public:
        icm20948::ICM20948_I2C& sensor;
        GPIOClass(const char* chipName, int InterruptPin, int LEDPin, icm20948::ICM20948_I2C& sensor);
        void Worker();
        bool running;
        void ReadAccel();
    };
}

#endif