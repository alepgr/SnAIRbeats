#include "libs/I2C/include/icm20948_i2c.hpp"
#include "libs/I2C/include/icm20948_defs.hpp"

#include "libs/GPIO/include/gpioevent.h"

#include "libs/ALSAAudio/include/AudioLib.hpp"

#include "libs/IMUMaths/include/IMUMaths.hpp"

#include "libs/PlayAudio/include/PlayAudio.hpp"

#include "libs/ALSAPlayer/include/ALSAPlayer.hpp"

#include <iostream> 
#include <iomanip>
#include <gpiod.h>

#include <chrono>
#include <thread>
#include <string>


#include <bitset>
#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 17

//Object for I2C operations
//Don't think needs closed

//Objects for each IMU sensor with respective I2C addresses from datasheet
//Allows for running on the same line
icm20948::ICM20948_I2C objI2C(1,0x69); 
icm20948::ICM20948_I2C objI2C_2(1,0x68); 

//Object for Playing audio
PlayAudioName::PlayAudio objAudio;

//Object for ALSA Audio Player
// AudioPlayerName::AudioPlayer objALSA("plughw:2,0", 44100, 2, SND_PCM_FORMAT_S16_LE, 128);
AudioPlayerName::AudioPlayer objALSA("plughw:CARD=UACDemoV10,DEV=0", 44100, 2, SND_PCM_FORMAT_S16_LE, 128);


//Object for Maths operations, passing values in
IMUMathsName::IMUMaths objMaths(objALSA);

//Object for GPIO operations
GPIOName::GPIOClass objGPIO("gpiochip0", 17, objI2C, objMaths);
GPIOName::GPIOClass objGPIO_2("gpiochip0",27, objI2C_2,objMaths);


void KeyboardInterrupt(std::thread &GPIOThread, std::thread &GPIOThread_2) {
    std::string input;
    while (true) {
        std::cout << "Press 'q' to quit: " << std::endl;
        std::cin >> input;
        if (input == "q" || input == "Q") {

            //Stop both GPIO worker threads and join them, then close ALSA object
            objGPIO.GPIOStop();
            objGPIO_2.GPIOStop();
            
            if(GPIOThread.joinable()){
                GPIOThread.join();
            }

            if(GPIOThread_2.joinable()){
                GPIOThread_2.join();
            }

            objALSA.close();

            std::cout << "Everything closed." << std::endl;
            return;
        }
    }
}



int main() {

    objALSA.open();
    
    // objALSA.playFile("src/libs/ALSAPlayer/include/SnareDrum.wav");
    // objALSA.playFile("src/libs/ALSAPlayer/include/CrashCymbal.wav");
    // objALSA.playFile("src/libs/ALSAPlayer/include/HighTom.wav");

    objI2C.settings.accel.sample_rate_div = 0;
    objI2C.settings.gyro.sample_rate_div = 0;
    objI2C.settings.accel.scale = icm20948::ACCEL_16G;
    objI2C.settings.gyro.scale = icm20948::GYRO_2000DPS;

    objI2C_2.settings.accel.sample_rate_div = 0;
    objI2C_2.settings.gyro.sample_rate_div = 0;
    objI2C_2.settings.accel.scale = icm20948::ACCEL_16G;
    objI2C_2.settings.gyro.scale = icm20948::GYRO_2000DPS;
    
    std::cout << "Object created!\n";
    if(objI2C.init())
    {
        std::cout << "Hurray!" << std::endl;

        if (objI2C.enable_DRDY_INT()){
            std::cout << "Data Ready Interrupt enabled" << std::endl;
        } else {
            std::cerr <<"Failed to enable DRDY interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky" << std::endl;
        return -1;
    }

    std::cout << "Object created!\n";
    if(objI2C_2.init())
    {
        std::cout << "Hurray!" << std::endl;

        if (objI2C_2.enable_DRDY_INT()){
            std::cout << "Data Ready Interrupt enabled" << std::endl;
        } else {
            std::cerr <<"Failed to enable DRDY interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky" << std::endl;
        return -1;
    }

    //Thread for IMU worker 1
    std::thread gpioThread(&GPIOName::GPIOClass::Worker, &objGPIO);
    std::thread gpioThread_2(&GPIOName::GPIOClass::Worker, &objGPIO_2);

    // Thread for keyboard interrupt
    std::thread keyboardThread(KeyboardInterrupt, std::ref(gpioThread), std::ref(gpioThread_2));



    //objGPIO.running = false;

    // Join threads to clean up properly.
    // if (gpioThread.joinable())
    //     gpioThread.join();

    //std::cout << "Hello" << std::endl;

    if (keyboardThread.joinable())
        keyboardThread.join();

    std::cout << "Exiting program" << std::endl;
    return 0;

}
