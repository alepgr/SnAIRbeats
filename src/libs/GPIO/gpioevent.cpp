#include "include/gpioevent.h"
#include <gpiod.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <cstdint>
#include <functional>
#include <gtest/gtest.h>

#include "../I2C/include/icm20948_i2c.hpp"
#include "../I2C/include/icm20948_utils.hpp"

#include "../IMUMaths/include/IMUMaths.hpp"

namespace GPIOName {

    GPIOClass::GPIOClass(const char* chipName, int InterruptPin,
         icm20948::ICM20948_I2C& sensor, IMUMathsName::IMUMaths& Maths)
        : chip(nullptr), SensorLine(nullptr), LEDLine(nullptr),
          InterruptPin(InterruptPin),
          running(true), Counter(0), sensor(sensor), Maths(Maths)
    {
        chip = gpiod_chip_open_by_name(chipName);
        if (!chip) {
            std::cerr << "[ERROR] Could not open chip - womp womp" << std::endl;
        }
        SensorLine = gpiod_chip_get_line(chip, InterruptPin);
        if (!SensorLine) {
            std::cerr << "[ERROR] Could not get sensor line" << std::endl;
        }
        if (gpiod_line_request_both_edges_events(SensorLine, "sensor") < 0) {
            std::cerr << "[ERROR] Could not request events for sensor line - womp womp" << std::endl;
        }

        //Registers the callback
        std::cout << "GPIO Initialised" << std::endl;

    }

    void GPIOClass::Worker() {
        bool Pause = false;
        while (running) {
            int ret = gpiod_line_event_wait(SensorLine, nullptr);
            if (ret < 0) {
                std::cerr << "Error waiting for event" << std::endl;
                continue;
            }
            if (ret > 0) {
                struct gpiod_line_event event;
                if (gpiod_line_event_read(SensorLine, &event) < 0) {
                    std::cerr << "Error reading event" << std::endl;
                    continue;
                }
                if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
                    //Tells IMU driver to read acceleration data from registers
                    if(!sensor.read_accel_gyro()){
                        std::cout << "read_accel_gyro() failed!\nexiting..." << std::endl;
                        break;
                    }

                    //Clears the interrupt latch once data has been read
                    sensor.check_DRDY_INT();

                    //Amazingly named callback, sends acceleration data from I2C driver into maths object
                    if (callback){
                        callback -> MathsCallback(sensor.accel[0],sensor.accel[1],sensor.accel[2]);
                    }
                }   
            }
        }
    };

    //Turns off the worker when called
    void GPIOClass::GPIOStop(){
        running = false;
    }

}