#ifndef IMUMATHS_H
#define IMUMATHS_H


#include <iostream> 
#include <array>
#include <iomanip>


namespace IMUMathsName {
    class IMUMaths{
        private:
        
        

        public:
        bool QuadraticSum(float X, float Y, float Z);
        std::array<float, 10>bufferX;
        std::array<float, 10>bufferY;
        std::array<float, 10>bufferZ;


    };
}


#endif