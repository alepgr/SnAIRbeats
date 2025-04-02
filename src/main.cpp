#include "libs/I2C/include/icm20948_i2c.hpp"
#include "libs/I2C/include/icm20948_defs.hpp"

#include "libs/GPIO/include/gpioevent.h"

#include "libs/cam2opencv/libcam2opencv.h"
#include "libs/cam2opencv/qtviewer/window.h"

#include "libs/ALSAAudio/include/AudioLib.hpp"

#include <iostream> 
#include <iomanip>
#include <gpiod.h>

#include <chrono>
#include <thread>

#include <bitset>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 17



struct MyCallback : Libcam2OpenCV::Callback {
    Window* window = nullptr;
    virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
        if (nullptr != window) {
            window->updateImage(frame);
        }
    }
};

int main()
{
    // Creating an instance of the class, inside the namespace
    // icm20948::ICM20948_I2C objI2C(1); // bus number 1 means it is communicating with an external device

    // GPIOName::GPIOClass objGPIO("gpiochip0", 17, 27);


    // // g_imu = &obj;

    // std::cout << "Object created!\n";
    // if(objI2C.init())
    // {
    //     std::cout << "Hurray!" << std::endl;

    //     if (objI2C.enable_DRDY_INT()){
    //         std::cout << "WOM Interrupt Enabled" << std::endl;
    //     } else {
    //         std::cerr <<"Failed to enable WOM interrupt" << std::endl;
    //     }
    // } else {
    //     std::cout << "Womp Womp - No worky" << std::endl;
    //     return -1;
    // }



    // std::thread gpioThread(&GPIOName::GPIOClass::Worker, &objGPIO);

    // //objGPIO.running = false;

    // // Join the thread to clean up properly.
    // if (gpioThread.joinable())
    //     gpioThread.join();

    // std::cout << "Exiting program" << std::endl;
    // return 0;
    


    //Test for PlayAudio

    try {
        AudioLib::AudioLib AudioLib("plughw:2,0");

        AudioLib.PlayFile();

        
    }
    catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

