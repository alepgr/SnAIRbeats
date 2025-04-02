#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

#include "I2C/include/icm20948_i2c.hpp"



namespace datacollection  {
    // Storage for data
    struct DataSample {
        double Xacc;
        double Yacc;
        double Zacc;
        double Xgyro ; 
        double Ygyro ; 
        double Zgyro ; 
        long timestamp; // Time in milliseconds
    };
    // Buffer for data
    DataSample dataBuffer[MAX_SAMPLES];

    bool recording = true;
    std::ofstream file("data.csv", std::ios::trunc);

    // Mock callback function (replace with your actual callback)
    void dataReadyCallback(double Xacc,double Yacc, doubleZacc,Xgyro,Ygyro,Zgyro) {
        if (recording) {
            // Get the time for timeptamp
            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
            dataBuffer.push_back(Xacc,Yacc,Zacc,Xgyro,Ygyro,Zgyro,now );
        }
    }

    int main() {

        icm20948::ICM20948_I2C objI2C(1);
        
        std::cout << "Recording data for 20 seconds..." << std::endl;

        // Start time
        auto start = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::seconds(20);

        // Simulated data collection loop
        while (std::chrono::high_resolution_clock::now() - start < duration) {
            // Simulate data reception (replace with actual callback trigger)
            double simulatedXacc = static_cast<double>(rand()) / RAND_MAX; // Example random data
            dataReadyCallback(simulatedXacc);

            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate sampling delay
        }

        recording = false; // Stop recording
        std::cout << "Recording finished. Saving to file..." << std::endl;

        objI2C.read_accel_gyro();

        void dataReadyCallback(double Xacc, double Yacc, double Zacc) {
            if (recording && sampleCount < MAX_SAMPLES) {
                auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::high_resolution_clock::now().time_since_epoch())
                               .count();
        
                // Store in array
                dataBuffer[sampleCount] = {Xacc, Yacc, Zacc, now};
                sampleCount++;
            }
        }


        // Save data to CSV
        
        if (file.is_open()) {
            file << "Timestamp,Xacc\n";
            for (size_t i = 0; i < dataBuffer.size(); ++i) {
                file << i * 10 << "," << dataBuffer[i] << "\n"; // Assuming 10ms interval
            }
            file.close();
            std::cout << "Data saved to data.csv" << std::endl;
        } else {
            std::cerr << "Error opening file!" << std::endl;
        }

        return 0;
    }
}