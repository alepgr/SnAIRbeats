#ifndef GPIOEVENT_H
#define GPIOEVENT_H

#include <gpiod.h>
#include <iostream>
#include "icm20948_i2c.hpp"

namespace GPIOName {
    class GPIOClass {
    private:
        gpiod_chip* chip;
        gpiod_line* SensorLine;
        gpiod_line* LEDLine;
        int InterruptPin;
        int LEDPin;

        int Counter;
        icm20948::ICM20948_I2C* sensor;

    public:
        GPIOClass(const char* chipName, int InterruptPin, int LEDPin);
        void Worker();
        bool running;
    };
}

#endif
