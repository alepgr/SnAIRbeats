#ifndef GPIOEVENT_H
#define GPIOEVENT_H


#include <gpiod.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <functional>
#include <gtest/gtest.h>

#include "../../I2C/include/icm20948_i2c.hpp"
#include "../../I2C/include/icm20948_utils.hpp"

#include "../../IMUMaths/include/IMUMaths.hpp"


namespace GPIOName {
    class GPIOClass {
    public:

        /** 
         * @brief Constructor for GPIOClass.
         * 
         * @param chipName The name of the GPIO chip (e.g., "gpiochip0")
         * @param InterruptPin The GPIO pin number for interrupts
         * @param sensor access to ICM20948_I2C objects 
         * @see icm20948::ICM20948_I2C
         */
        GPIOClass(const char* chipName, int InterruptPin,
             icm20948::ICM20948_I2C& sensor);

        /**
         * @brief Event driven worker reading data when HIGH seen on GPIO
         * 
         * This function is an event driven interrupt controlled by a GPIO pin.
         * Once this GPIO pin reads HIGH the function will read the data registers
         * using the ReadAccel() callback from the IMU's driver which is then fed into the 
         * IMU Maths object to be analysed.
         */
        void Worker();

        /**
         * @brief Changes a boolean to end the worker
         */
        void GPIOStop();

        /**
         * @brief Empty callback to later be filled. Includes destructor
         */
        struct Callback{
            virtual void MathsCallback(float X, float Y, float Z) = 0;
            virtual ~Callback(){};
        };

        /**
         * @brief Registers a callback
         * 
         *  Overwrites the virtual void function within the class with a function taken in via this function
         * 
         * @param cb callback to register
         */
        void RegisterCallback(Callback* cb){
            callback = cb;
        }

        //Testing private stuff
        #ifdef UNIT_TEST
            bool GetRunning() const {
                return running.load();
            }

            bool HasCallback() const {
                return callback != nullptr;
            }

            Callback* GetCallback() const {
                return callback;
            }

        #endif

        private:
        gpiod_chip* chip;
        gpiod_line* SensorLine;
        int InterruptPin;
        int Counter;
        bool Pause = true;

        //Reference to IMU's driver
        icm20948::ICM20948_I2C& sensor;
        
        std::atomic<bool> running{true};
        Callback* callback = nullptr;
        
    };

    struct MathsCallbackStruct : GPIOName::GPIOClass::Callback{
        IMUMathsName::IMUMaths& Maths;

        MathsCallbackStruct(IMUMathsName::IMUMaths& maths) : Maths(maths) {}

        virtual void MathsCallback(float X, float Y, float Z) override {
            Maths.SoundChecker(X, Y, Z);
        }
    };
}



#endif
