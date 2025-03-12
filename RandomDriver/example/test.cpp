// Custom headers to define communication with the IMU sensor
#include "icm20948_i2c.hpp"
#include "icm20948_defs.hpp"

#include <iostream> // "input output" stream, to print messages
#include <iomanip> // to "manipulate"/format the output messages
#include <gpiod.h>

// Timing operations
#include <chrono>
#include <thread>

// For printing binary values (used in commented out debugging code)
#include <bitset>


#define INT_PIN 27

//Global pointer so the int handler can access sensor instance
icm20948::ICM20948_I2C* g_imu = nullptr;
gpiod_chip *chip;
gpiod_line *line;

//Hardware Int callback
void gpio_Int_Handler(int event, void* args){
    //When GPIO interrupt fires, check DRDY flag
    if (g_imu && g_imu -> check_DRDY_INT()){
        std::cout<<"Data Ready, Interrupt Triggered"<< std::endl;

        if (!g_imu->read_accel_gyro()) {
            std::cout << "Accel: X=" << g_imu->accel[0] << " Y=" << g_imu->accel[1] << " Z=" << g_imu->accel[2] << "\n";

        }else {
            std::cout <<"Int fired, but no data?? Womp Womp"<<std::endl;
        }

    }else {
        std::cout <<"Int fired, but no data?? Womp Womp"<<std::endl;
    }
}

int main()
{
    // Creating an instance of the class, inside the namespace
    icm20948::ICM20948_I2C obj(1); // bus number 1 means it is communicating with an external device

    g_imu = &obj;

    // obj.settings.accel.sample_rate_div = 4; // sets the sampling rate to reduce noise
    // obj.settings.gyro.sample_rate_div = 4;
    // obj.settings.accel.dlpf_enable = 1;
    // obj.settings.accel.scale = icm20948::ACCEL_2G;
    // obj.settings.accel.dlpf_config = icm20948::ACCEL_DLPF_111_4HZ;
    // obj.settings.gyro.dlpf_enable = 1;
    // obj.settings.gyro.scale = icm20948::GYRO_500DPS;
    // obj.settings.gyro.dlpf_config = icm20948::GYRO_DLPF_151_8HZ;

    std::cout << "Object created!\n";
    if(obj.init())
    {
        std::cout << "Hurray!" << std::endl;

        if (obj.enable_wom_interrupt(0x04)){
            std::cout << "WOM Interrupt Enabled" << std::endl;
        } else {
            std::cerr <<"Failed to enable WOM interrupt" << std::endl;
        }
    } else {
        std::cout << "Womp Womp - No worky" << std::endl;
        return -1;
    }

/*
    //Data Ready Interrupt
    if(!obj.enable_DRDY_INT()) {
        std::cerr << "[ERROR] Failed to enable DRDY\n";
        return -1;
    }

    std::cout << "DRDY Int Enabled/n";



    // Set up GPIO hardware int
    chip = gpiod_chip_open_by_name("gpiochip0"); //Open GPIO device
    if (!chip) {
        std::cerr << "[ERROR] Failed to open GPIO chip!\n";
        return -1;
    }

    line = gpiod_chip_get_line(chip, INT_PIN);
    if (!line) {
        std::cerr << "[ERROR] Failed to get GPIO line!\n";
        return -1;
    }

    if (gpiod_line_request_falling_edge_events(line, "icm20948") < 0) {
        std::cerr << "[ERROR] Failed to configure GPIO for interrupts!\n";
        return-1;
    }

    std::cout << "[INFO] GPIO interrupt set to pin " << INT_PIN << "." << std::endl;
    std::cout << "[INFO] Waiting for Data Ready Interrupt... " << std::endl;

    struct gpiod_line_event event;
    while(true) {
        if (gpiod_line_event_wait(line,NULL) > 0) {
            gpiod_line_event_read(line, &event);
            gpio_Int_Handler(event.event_type, nullptr);

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    gpiod_chip_close(chip);
    return 0;

*/
    //Enable WOM with threshold - 32mg



    // uint8_t lsb, msb, gyro;
    // if(obj._read_byte(ICM20948_ACCEL_SMPLRT_DIV_1_BANK, ICM20948_ACCEL_SMPLRT_DIV_1_ADDR, msb) &&
    //    obj._read_byte(ICM20948_ACCEL_SMPLRT_DIV_2_BANK, ICM20948_ACCEL_SMPLRT_DIV_2_ADDR, lsb) &&
    //    obj._read_byte(ICM20948_GYRO_SMPLRT_DIV_BANK,    ICM20948_GYRO_SMPLRT_DIV_ADDR,    gyro))
    // {
    //     std::cout << "Accel:\n"
    //                  "  MSB: " << std::bitset<8>(msb) << std::endl <<
    //                  "  LSB: " << std::bitset<8>(lsb) << std::endl <<
    //                  "Gyro:\n"
    //                  "  BYTE: " << std::bitset<8>(gyro) << std::endl;
    // }
    // else
    // {
    //     std::cout << "Umm, what?\n";
    // }

    // uint8_t accel_config_1, gyro_config_1;
    // if(obj._read_byte(ICM20948_ACCEL_CONFIG_1_BANK, ICM20948_ACCEL_CONFIG_1_ADDR, accel_config_1) &&
    //    obj._read_byte(ICM20948_GYRO_CONFIG_1_BANK,  ICM20948_GYRO_CONFIG_1_ADDR,  gyro_config_1))
    // {
    //     std::cout << "Accel config 1: " << std::bitset<8>(accel_config_1) << std::endl <<
    //                  "Gyro  config 1: " << std::bitset<8>(gyro_config_1)  << std::endl;
    // }
    // else
    // {
    //     std::cout << "Umm, what?\n";
    // }



    while (true) {
        //Wake up if motion
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (obj.check_wom_interrupt()) {
            std::cout << "Motion detected!!" <<std::endl;

            if (!obj.read_accel_gyro()) {
                std::cout << "read_accel_gyro() failed!\nWomp Womp" << std::endl;
                break;
            }

            if (!obj.read_magn()) {
                std::cout << "read_magn() failed\nWomp Womp" << std::endl;
                break;
            }
        }
    }

/*
    
    while(true)
    {
        // auto start = std::chrono::high_resolution_clock::now();
        if(!obj.read_accel_gyro())
        {
            std::cout << "read_accel_gyro() failed!\nexiting..." << std::endl;
            break;
        }
        // auto stop = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        // std::cout << "Time needed to parse accel/gyro data: " << duration.count() << "us" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        if(!obj.read_magn())
        {
            std::cout << "read_accel_gyro() failed!\nexiting..." << std::endl;
            break;
        }
        // stop = std::chrono::high_resolution_clock::now();
        // duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        // std::cout << "Time needed to parse magn data: " << duration.count() << "us" << std::endl;

        
        std::cout << std::setprecision(2) << "Accel:\n"
                                             "  x = " << obj.accel[0] << std::endl <<
                                             "  y = " << obj.accel[1] << std::endl <<
                                             "  z = " << obj.accel[2] << std::endl << std::endl;
        /*std::cout << std::setprecision(2) << "Gyro:\n"
                                             "  x = " << obj.gyro[0] << std::endl <<
                                             "  y = " << obj.gyro[1] << std::endl <<
                                             "  z = " << obj.gyro[2] << std::endl << std::endl;
        std::cout << std::setprecision(2) << "Magn:\n"
                                             "  x = " << obj.magn[0] << std::endl <<
                                             "  y = " << obj.magn[1] << std::endl <<
                                             "  z = " << obj.magn[2] << std::endl << std::endl; 
                                             
                                             
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << std::endl;
    }    */
    
}