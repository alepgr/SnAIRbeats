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

#include "../../I2C/include/icm20948_i2c.hpp"
#include "../../I2C/include/icm20948_utils.hpp"

#include "../../IMUMaths/include/IMUMaths.hpp"


namespace GPIOName {

    typedef void (*GPIOCallback)(void* context, float, float, float);
    class GPIOClass {
    public:
        icm20948::ICM20948_I2C& sensor;
        IMUMathsName::IMUMaths& Maths;
        
        // GPIOCallback callback;
        // void* CallbackFunction;

        // function added for testing
        bool IsRunning() const {
            return running.load();
        }

        /** 
         * @brief Constructor for GPIOClass.
         * 
         * @param chipName The name of the GPIO chip (e.g., "gpiochip0")
         * @param InterruptPin The GPIO pin number for interrupts
         * @param sensor access to ICM20948_I2C objects ( @see icm20948::ICM20948_I2C)
         * @param Maths access to IMUMaths objects ( @see IMUMathsName::IMUMaths)
         */
        GPIOClass(const char* chipName, int InterruptPin,
             icm20948::ICM20948_I2C& sensor, IMUMathsName::IMUMaths& Maths);

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

        /**
         * @brief Changes a boolean to end the worker
         */
        void GPIOStop();

        /**
         * @brief Callback using virtual void
         */
        struct Callback{
            virtual void MathsCallback(float X, float Y, float Z) = 0;
            virtual ~Callback(){};
        };

        /**
         * @brief Registers a callback
         * 
         * @param cb callback to register
         */
        void RegisterCallback(Callback* cb){
            callback = cb;
        }
        
        // void SetCallback(GPIOCallback cb, void* context);
        // static void IMUMathsCallback(void* context, float X, float Y, float Z){
        //     IMUMathsName::IMUMaths* maths = static_cast<IMUMathsName::IMUMaths*>(context);
        //     maths->SoundChecker(X,Y,Z);
        // }



        private:
        gpiod_chip* chip;
        gpiod_line* SensorLine;
        gpiod_line* LEDLine;
        int InterruptPin;
        int Counter;
        bool Pause = true;
        
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
