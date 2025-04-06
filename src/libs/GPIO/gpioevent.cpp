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

#include "../IMUMaths/include/IMUMaths.hpp"


namespace GPIOName {

    GPIOClass::GPIOClass(const char* chipName, int InterruptPin, int LEDPin, icm20948::ICM20948_I2C& sensor, IMUMathsName::IMUMaths& Maths)
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

    void GPIOClass::WorkerDataCollect() {
        // CSV file create
        std::string filename;
        std::cout<<"Enter filename to save sensor data:";
        std::getline(std::cin, filename);
        if(filename.empty()){
            filename = "sensor_data"; // Default filename if no user input
        }
        filename+=".csv"; // Add extension to filename
        std::ofstream file(filename, std::ios::trunc); // If the file exists erases the data first
        file << "Timestamp (µs), Xacc, Yacc, Zacc, Xgyro, Ygyro, Zgyro\n"; //Hearde of the file 
        file.close(); //Close CSV file
        std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now(); //Recording start time




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
                    uint32_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start_time).count();
                    std::cout << Counter << " Hits have been detected" << std::endl;
                    
                    sensor.check_DRDY_INT();

                    if(!sensor.read_accel_gyro()){
                        std::cout << "read_accel_gyro() failed!\nexiting..." << std::endl;
                        break;
                    }

                    // std::cout << std::setprecision(4)
                    //           << "Accel:\n"
                    //           << "  x = " << sensor.accel[0] << std::endl
                    //           << "  y = " << sensor.accel[1] << std::endl
                    //           << "  z = " << sensor.accel[2] << std::endl << std::endl;

                    // std::cout << std::setprecision(4)
                    //           << "Gyro:\n"
                    //           << "  x = " << sensor.gyro[0] << std::endl
                    //           << "  y = " << sensor.gyro[1] << std::endl
                    //           << "  z = " << sensor.gyro[2] << std::endl << std::endl;

                            std::cout<<filename<<std::endl;
                              file.open(filename, std::ios::app); //Open file to add new data 
                              if (!file) {
                                  std::cerr << "Error opening file!" << std::endl;
                                  return;
                              }
                              file << timestamp<< "," <<sensor.accel[0] << "," <<sensor.accel[1]<< "," <<sensor.accel[2] << "," <<sensor.gyro[0] << "," <<sensor.gyro[1]  << "," <<sensor.gyro[2]  << "\n";  // Write a new line in the csv
                              file.close(); //Close CSV file

                    if (sensor.accel[0] >= -72){
                        std::cout << "making noise" << std::endl;
                       int ret = system("aplay -D plughw:2,0 ../libs/ALSAAudio/AudioFiles/SnareDrum1.wav");
                    }
                }
            }
        }
    }

    void GPIOClass::Worker() {
        bool Pause = false;
        int Delay = 200;
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
                    //std::cout << Counter << " Hits have been detected" << std::endl;
                    
                    sensor.check_DRDY_INT();

                    if(!sensor.read_accel_gyro()){
                        std::cout << "read_accel_gyro() failed!\nexiting..." << std::endl;
                        break;
                    }

                    // std::cout << std::setprecision(4)
                    //           << "Accel:\n"
                    //           << "  x = " << sensor.accel[0] << std::endl
                    //           << "  y = " << sensor.accel[1] << std::endl
                    //           << "  z = " << sensor.accel[2] << std::endl << std::endl;

                    // std::cout << std::setprecision(4)
                    //           << "Gyro:\n"
                    //           << "  x = " << sensor.gyro[0] << std::endl
                    //           << "  y = " << sensor.gyro[1] << std::endl
                    //           << "  z = " << sensor.gyro[2] << std::endl << std::endl;
                    

                    //Change to case switch and make own class
                    if (Pause == false){
                        if (sensor.accel[0] <=-40 && sensor.accel[0] >=-45){
                            std::thread soundThread(&GPIOClass::PlaySound, this);
                            soundThread.detach();
                            //std::this_thread::sleep_for(std::chrono::milliseconds(125));
                            Pause = true;
                            Counter = 0;
                        } else if (sensor.accel[1] <=-40 && sensor.accel[1] >= -45){
                            std::thread soundThread(&GPIOClass::PlaySoundHighTom, this);
                            soundThread.detach();
                            Pause = true;
                            Counter = 0;
                        } else if (sensor.accel[2] <= 20 && sensor.accel[2] >= 15){
                            std::thread soundThread(&GPIOClass::PlaySoundCymbal, this);
                            soundThread.detach();
                            Pause = true;
                            Counter = 0;
                        }
                    } else if (Pause == true){
                        Counter ++;
                        if (Counter == delay){
                            Pause = false;
                        }
                    }
                }
            }
        }
    }

    void GPIOClass::PlaySound() {
        //int retkill = system("killall aplay");
        //int ret = system("aplay -D plughw:0,0 src/libs/GPIO/include/SnareDrum2.wav &");
        int ret = system("aplay -D pulse src/libs/GPIO/include/SnareDrum3.wav");
    }

    void GPIOClass::PlaySoundCymbal() {
        //int retkill = system("killall aplay");
        //int ret = system("aplay -D plughw:0,0 src/libs/GPIO/include/SnareDrum2.wav &");
        int ret = system("aplay -D pulse src/libs/GPIO/include/CrashCymbal.wav");
    }

    void GPIOClass::PlaySoundHighTom() {
        //int retkill = system("killall aplay");
        //int ret = system("aplay -D plughw:0,0 src/libs/GPIO/include/SnareDrum2.wav &");
        int ret = system("aplay -D pulse src/libs/GPIO/include/HighTom.wav");
    }
}