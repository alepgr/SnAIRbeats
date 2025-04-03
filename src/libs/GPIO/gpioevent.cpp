#include "include/gpioevent.h"
#include <gpiod.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <cstdint>

#include "../I2C/include/icm20948_i2c.hpp"
#include "../I2C/include/icm20948_utils.hpp"


namespace GPIOName {

    GPIOClass::GPIOClass(const char* chipName, int InterruptPin, int LEDPin, icm20948::ICM20948_I2C& sensor)
        : chip(nullptr), SensorLine(nullptr), LEDLine(nullptr),
          InterruptPin(InterruptPin), LEDPin(LEDPin),
          running(true), Counter(0), sensor(sensor) 
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
    }

    void GPIOClass::Worker() {
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
                    Counter++;
                    std::cout << Counter << " Hits have been detected" << std::endl;
                    
                    sensor.check_DRDY_INT();

                    if(!sensor.read_accel_gyro()){
                        std::cout << "read_accel_gyro() failed!\nexiting..." << std::endl;
                        break;
                    }

                    std::cout << std::setprecision(4)
                              << "Accel:\n"
                              << "  x = " << sensor.accel[0] << std::endl
                              << "  y = " << sensor.accel[1] << std::endl
                              << "  z = " << sensor.accel[2] << std::endl << std::endl;

                    std::cout << std::setprecision(4)
                              << "Gyro:\n"
                              << "  x = " << sensor.gyro[0] << std::endl
                              << "  y = " << sensor.gyro[1] << std::endl
                              << "  z = " << sensor.gyro[2] << std::endl << std::endl;
                }
            }
        }
    }
}