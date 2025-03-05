//This file is an interrupt handler for the IMU that will activate other classes
//once a large enough data is seen. This will run on its own class

#include "RandomDriver/icm20948_i2c.hpp"
#include "RandomDriver/icm20948_defs.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <wiringpi.h>  // For GPIO interrupt handling

// Define the GPIO pin for the IMU interrupt signal
#define INTERRUPT_PIN 0  // GPIO pin 17 (wiringPi pin 0)

class IMUInterruptHandler {
public:
    IMUInterruptHandler(int imuBus): imuBus(imuBus), running(true){
        wiringPiSetup();
        pinMode(INTERRUPT_PIN, INPUT);
    }
}