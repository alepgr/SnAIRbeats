#ifndef DATACOLLECT_H
#define DATACOLLECT_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "../I2C/include/icm20948_i2c.hpp"
#include "../GPIO/include/gpioevent.h"

class Datacollection {
public:
    
    Datacollection(icm20948::ICM20948_I2C &sensorObj);

    std::string CreateCSVfile();

    uint32_t GetTimestamp(std::chrono::steady_clock::time_point start_time);

    void NewDataApp(const std::string &filename, uint32_t timestamp,
                    double Xacc, double Yacc, double Zacc,
                    double Xgyro, double Ygyro, double Zgyro);

    void collectAndLogData(const std::string &filename, std::chrono::steady_clock::time_point start_time);

private:
    // Reference to the sensor object, allowing access to its member functions.
    icm20948::ICM20948_I2C &sensor;
};

#endif // DATACOLLECTION_H
