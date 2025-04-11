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

#include "../../IMUMaths/include/IMUMaths.hpp"

#include "../../PlayAudio/include/PlayAudio.hpp"

namespace GPIOName {
    class GPIOClass {
    private:
        gpiod_chip* chip;
        gpiod_line* SensorLine;
        gpiod_line* LEDLine;
        int InterruptPin;
        int LEDPin;
        int Counter;
        bool Pause = true;
        int delay = 224;
    
    public:
        icm20948::ICM20948_I2C& sensor;
        IMUMathsName::IMUMaths& Maths;

        //Constructor
        GPIOClass(const char* chipName, int InterruptPin, int LEDPin,
             icm20948::ICM20948_I2C& sensor, IMUMathsName::IMUMaths& Maths);

        /**
         * @brief Event driven worker reading data when HIGH seen on GPIO
         * 
         * This function using blocking interrupts controlled by a GPIO pin.
         * Once this GPIO pin reads HIGH the function will read the data registers
         * using the ReadAccel() callback from the IMU's driver which is then fed into the 
         * IMU Maths object to be analysed.
         */
        void Worker();

        /**
         * @brief Event driven worker reading data when HIGH seen on GPIO and records data to a CSV
         * 
         * The function begins by initialising a csv file named by the user.
         * This function then uses blocking interrupts controlled by a GPIO pin.
         * Once this GPIO pin reads HIGH the function reads the data registers using
         * the ReadAccel() callback from the IMU's driver and appends this data into the 
         * opened CSV file.
         * 
         */
        void WorkerDataCollect();
        bool running;

        
        void PlaySound();
        void PlaySoundCymbal();
        void PlaySoundHighTom();
    };
}

#endif