#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>


class Datacollection {
public:
    void CreateCSVfile(){    // Create a csv file to safe data 
        std::string filename;
        std::cout<<"Enter filename to save sensor data:";
        std::getline(std::cin, filename);
        if(filename.empty()){
            filename = "sensor_data"; // Default filename if no user input
        }
        filename+=".csv"; // Add extension to filename
        std::ofstream file(filename, std::ios::trunc); // If the file exists erases the data first
        file << "Timestamp (µs), Xacc, Yacc, Zacc, Xgyro, Ygyro, Zgyro\n"; //Hearde of the file 
        file.close();
        return filename;
    }
    void GetTimestamp(auto start_time){
        return uint32_t timestamp = std::chrono::duration_cast<<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start_time).count();
    }
    void NewDataApp(std::string filename ,uint32_t timestamp,double Xacc,double Yacc, double Zacc,double Xgyro,double Ygyro,double Zgyro){
        std::ofstream file;
        file.open(filename, std::ios::app); //Open file to add new data 
        if (file.is_open()) {
            file << timestamp<< "," <<Xacc << "," <<Yacc << "," <<Zacc << "," <<Xgyro << "," <<Ygyro << "," <<Zgyro << "\n";  // Write a new line in the csv
            file.close();
        } else {
            std::cerr << "Error opening file!" << std::endl;
        }

    }
}