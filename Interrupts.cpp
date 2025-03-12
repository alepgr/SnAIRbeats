#include "RandomDriver/icm20948_i2c.hpp"
#include "RandomDriver/icm20948_defs.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <gpiod.h> // For GPIO interrupt handling

#define GPIO_CHIP "/dev/gpiochip0" // Default GPIO chip path
#define INTERRUPT_PIN 17           // GPIO pin number (not WiringPi numbering)

class IMUInterruptHandler {
public:
    IMUInterruptHandler(int imuBus) : imuBus(imuBus), running(true) {
        chip = gpiod_chip_open(GPIO_CHIP);
        if (!chip) {
            std::cerr << "Failed to open GPIO chip" << std::endl;
            running = false;
            return;
        }
        
        line = gpiod_chip_get_line(chip, INTERRUPT_PIN);
        if (!line) {
            std::cerr << "Failed to get GPIO line" << std::endl;
            gpiod_chip_close(chip);
            running = false;
            return;
        }
        
        if (gpiod_line_request_falling_edge_events(line, "imu_interrupt_handler") < 0) {
            std::cerr << "Failed to request GPIO interrupt" << std::endl;
            gpiod_chip_close(chip);
            running = false;
            return;
        }
        
        obj = std::make_unique<icm20948::ICM20948_I2C>(imuBus);
        if (obj->init()) {
            std::cout << "IMU Initialized successfully!" << std::endl;
        } else {
            std::cerr << "Failed to initialize IMU." << std::endl;
            running = false;
        }
        
        interruptThread = std::thread(&IMUInterruptHandler::interruptHandler, this);
    }

    ~IMUInterruptHandler() {
        running = false;
        if (interruptThread.joinable()) {
            interruptThread.join();
        }
        gpiod_line_release(line);
        gpiod_chip_close(chip);
    }

private:
    int imuBus;
    bool running;
    std::thread interruptThread;
    std::unique_ptr<icm20948::ICM20948_I2C> obj;
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    
    void interruptHandler() {
        struct gpiod_line_event event;
        while (running) {
            if (gpiod_line_event_wait(line, nullptr) > 0) {
                if (gpiod_line_event_read(line, &event) == 0) {
                    handleDataRead();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        }
    }
    
    void handleDataRead() {
        if (!obj->read_accel_gyro()) {
            std::cerr << "Failed to read accelerometer and gyroscope." << std::endl;
            return;
        }
        if (!obj->read_magn()) {
            std::cerr << "Failed to read magnetometer." << std::endl;
            return;
        }
        std::cout << std::setprecision(2) << "Accel:\n"
                  << "  x = " << obj->accel[0] << "\n"
                  << "  y = " << obj->accel[1] << "\n"
                  << "  z = " << obj->accel[2] << "\n"
                  << "Gyro:\n"
                  << "  x = " << obj->gyro[0] << "\n"
                  << "  y = " << obj->gyro[1] << "\n"
                  << "  z = " << obj->gyro[2] << "\n"
                  << "Magn:\n"
                  << "  x = " << obj->magn[0] << "\n"
                  << "  y = " << obj->magn[1] << "\n"
                  << "  z = " << obj->magn[2] << "\n";
    }
};

int main() {
    IMUInterruptHandler imuHandler(1);
    std::cout << "IMU Interrupt Handler is now running. Press Ctrl+C to exit." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
