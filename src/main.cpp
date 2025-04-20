#include "libs/I2C/include/icm20948_i2c.hpp"
#include "libs/I2C/include/icm20948_defs.hpp"

#include "libs/GPIO/include/gpioevent.h"
#include "libs/IMUMaths/include/IMUMaths.hpp"
#include "libs/ALSAPlayer/include/ALSAPlayer.hpp"

#include <iostream> 
#include <gpiod.h>
#include <thread>
#include <string>



//Driver objects for the IMUs
//Using difference I2C addresses allows them to run on the same I2C bus
icm20948::ICM20948_I2C objI2C(1,0x69); 
icm20948::ICM20948_I2C objI2C_2(1,0x68); 

//Object for ALSA Audio Player
AudioPlayerName::AudioPlayer objALSA("plughw:CARD=UACDemoV10,DEV=0", 44100, 2, SND_PCM_FORMAT_S16_LE, 128);

//Object for Maths operations
IMUMathsName::IMUMaths objMaths;

//Object for GPIO operations
//17 and 27 refer to GPIO pins on the raspberry pi (11 and 13)
//Require access the IMU's I2C driver for access to registers
GPIOName::GPIOClass objGPIO("gpiochip0", 17, objI2C);
GPIOName::GPIOClass objGPIO_2("gpiochip0",27, objI2C_2);


//All initialisation for SnairBeats
bool InitSnairBeat(){

    //Sample rate is divided by these values
    //1.125kHz / (1 + sample_rate_div)

    int SampleRateDivider = 44;
    // 1125/(44 + 1) = 25Hz

    objI2C.settings.accel.sample_rate_div = SampleRateDivider;
    objI2C.settings.gyro.sample_rate_div = SampleRateDivider;
    objI2C.settings.accel.scale = icm20948::ACCEL_16G;
    objI2C.settings.gyro.scale = icm20948::GYRO_2000DPS;

    objI2C_2.settings.accel.sample_rate_div = SampleRateDivider;
    objI2C_2.settings.gyro.sample_rate_div = SampleRateDivider;
    objI2C_2.settings.accel.scale = icm20948::ACCEL_16G;
    objI2C_2.settings.gyro.scale = icm20948::GYRO_2000DPS;
    
    std::cout << "Object 1 created!\n";
    if(objI2C.init())
    {

        if (objI2C.enable_DRDY_INT()){
            std::cout << "Data Ready Interrupt enabled in Object 1" << std::endl;
        } else {
            std::cerr <<"Failed to enable DRDY interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky in Object 1" << std::endl;
        return -1;
    }

    std::cout << "Object 2 created!\n";
    if(objI2C_2.init())
    {
        if (objI2C_2.enable_DRDY_INT()){
            std::cout << "Data Ready Interrupt enabled in Object 2" << std::endl;
        } else {
            std::cerr <<"Failed to enable DRDY interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky in Object 2" << std::endl;
        return -1;
    }

    if (!objALSA.open()) {
        std::cerr << "[ALSAPlayer] Warning: Failed to open ALSA device.\n";
    }

    if (!objALSA.startMixer()){
        std::cerr << "[ALSAPlayer] Warning: Failed to start mixing thread\n";

    }

    return 1;
}



int main() {

    bool success = true;

    success &= InitSnairBeat();
    if (!success){
        std::cerr << "Something failed in initialisation - go fix it" << std::endl;
        return -1;
    }
    


    //Register callback for Audio
    IMUMathsName::AudioCallback Audio_Callback(objALSA);
    objMaths.RegisterCallback(&Audio_Callback);

    //Register callback for Maths in the GPIO objects
    GPIOName::MathsCallbackStruct Maths_Callback(objMaths);
    objGPIO.RegisterCallback(&Maths_Callback);
    objGPIO_2.RegisterCallback(&Maths_Callback);
    

    //Thread for IMU workers 
    std::thread gpioThread(&GPIOName::GPIOClass::Worker, &objGPIO);
    std::thread gpioThread_2(&GPIOName::GPIOClass::Worker, &objGPIO_2);
    
    //Close down the program after Enter is pressed
    std::cout << "Press Enter to stop SnairBeats" << std::endl;
    getchar();

    //Swaps running atomic boolean in GPIO objects to false
    //Stopping them next iteration
    objGPIO.GPIOStop();
    objGPIO_2.GPIOStop();
            
    if(gpioThread.joinable()){
        gpioThread.join();
        }

    if(gpioThread_2.joinable()){
        gpioThread_2.join();
        }

    objALSA.stopMixer();
    
    std::cout << "Everything closed.\nExiting SnairBeat" << std::endl;

    return 0;

}
