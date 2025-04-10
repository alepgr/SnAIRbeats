#include "libs/I2C/include/icm20948_i2c.hpp"
#include "libs/I2C/include/icm20948_defs.hpp"

#include "libs/GPIO/include/gpioevent.h"

// #include "libs/cam2opencv/libcam2opencv.h"
// #include "libs/cam2opencv/qtviewer/window.h"

#include "libs/ALSAAudio/include/AudioLib.hpp"

#include "libs/IMUMaths/include/IMUMaths.hpp"

#include "libs/PlayAudio/include/PlayAudio.hpp"

#include "libs/CameraMaths/include/CameraMaths.hpp"

//#include "libs/DataCollection/include/DataCollection.hpp"

#include <iostream> 
#include <iomanip>
#include <gpiod.h>

#include <chrono>
#include <thread>

#include <bitset>

#include <QApplication>
#include <QProcess>
#include <QDebug>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 17







// struct MyCallback : Libcam2OpenCV::Callback {
//     Window* window = nullptr;
//     virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
//         if (nullptr != window) {
//             window->updateImage(frame);
//         }

//         if (frame.empty()) {
//             std::cerr << "Warning: Received an empty frame!" << std::endl;
//             return;
//         }
        //debugging stuff
    //     std::cout << "First row pixel values: ";
    //     int numPrint = std::min(frame.cols, 10); // Print at most 10 pixels for brevity.
    //     for (int j = 0; j < numPrint; j++) {
    //         cv::Vec3b pixel = frame.at<cv::Vec3b>(0, j);
    //         std::cout << "[" << (int)pixel[0] << ", " 
    //                   << (int)pixel[1] << ", " 
    //                   << (int)pixel[2] << "] ";
    //     }

    //     cv::imshow("Camera Frame", frame);
    //     cv::waitKey(1);

//     }
// };

// Libcam2OpenCV camera;


int main(int argc, char *argv[])
//int main()
{   
    QApplication app(argc, argv);
    //Camera Object
CameraMathsName::CameraMaths objCam;

//Object for I2C operations
icm20948::ICM20948_I2C objI2C(1); // bus number 1 means it is communicating with an external device

//Object for Playing audio
PlayAudioName::PlayAudio objAudio;

//Object for Maths operations, passing values in
IMUMathsName::IMUMaths objMaths(&objAudio);

//Object for GPIO operations
GPIOName::GPIOClass objGPIO("gpiochip0", 17, 27, objI2C, objMaths, objCam);

    CameraMathsName::CameraMaths::Screenshot shot;
    shot.window = &objCam.window;
    
    objCam.camera.registerCallback(&shot);
    // Set up for QT viewer, probably not needed for final model
    // QApplication app(argc, argv);
    //Libcam2OpenCV camera;
    //MyCallback MyCallback;
   //camera.registerCallback(&MyCallback);
    //camera.start();
    // Creating an instance of the class, inside the namespace
    // Window window;
    // window.show();
    //QProcess process;



    //process.start("libs/cam2opencv/qtviewer/qtviewer", QStringList());

    // if (!process.waitForStarted()){
    //     qDebug() << "Cannae open the window";
    // }

    // g_imu = &obj;


    // MyCallback MyCallback;

    // camera.registerCallback(&MyCallback);

    // camera.start();
  

    objI2C.settings.accel.sample_rate_div = 0;
    objI2C.settings.gyro.sample_rate_div = 0;
    objI2C.settings.accel.scale = icm20948::ACCEL_16G;
    objI2C.settings.gyro.scale = icm20948::GYRO_2000DPS;
    
    std::cout << "Object created!\n";
    if(objI2C.init())
    {
        std::cout << "Hurray!" << std::endl;

        if (objI2C.enable_DRDY_INT()){
            std::cout << "WOM Interrupt Enabled" << std::endl;
        } else {
            std::cerr <<"Failed to enable WOM interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky" << std::endl;
        return -1;
    }

 

    
    //objGPIO.CreateCSVfile();
    std::thread gpioThread(&GPIOName::GPIOClass::Worker, &objGPIO);

    // Run the QT viewer
    //app.exec();


    //objGPIO.running = false;

    // Join the thread to clean up properly.
    if (gpioThread.joinable())
        gpioThread.join();

    std::cout << "Exiting program" << std::endl;
    return 0;
    


    // Test for PlayAudio

    try {
        AudioLib::AudioLib AudioLib("plughw:2,0");

        AudioLib.PlayFile();

        
    }
    catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
    int ret = system("aplay -D plughw:2,0 src/libs/ALSAAudio/AudioFiles/SnareDrum1.wav");
    int ret1 = system("aplay -D plughw:2,0 src/libs/ALSAAudio/AudioFiles/SnareDrum1.wav");
    return 0;

    
    std::cout << "aplay returned: " << ret << std::endl;
}

// icm20948::ICM20948_I2C obj(1);

//     int main (){
//     icm20948::ICM20948_I2C obj(1);
//     while(true)
//     {
//         // auto start = std::chrono::high_resolution_clock::now();
        // if(!obj.read_accel_gyro())
        // {
        //     std::cout << "read_accel_gyro() failed!\nexiting..." << std::endl;
        //     break;
        // }

        // std::cout << std::setprecision(4) << "Accel:\n"
        //                                      "  x = " << obj.accel[0]  << std::endl <<
        //                                      "  y = " << obj.accel[1]  << std::endl <<
        //                                      "  z = " << obj.accel[2] << std::endl << std::endl;
//         // std::cout << std::setprecision(2) << "Gyro:\n"
//         //                                      "  x = " << obj.gyro[0] << std::endl <<
//         //                                      "  y = " << obj.gyro[1] << std::endl <<
//         //                                      "  z = " << obj.gyro[2] << std::endl << std::endl;
//         //std::this_thread::sleep_for(std::chrono::milliseconds(200));
//         std::cout << std::endl;
//     }
// }
