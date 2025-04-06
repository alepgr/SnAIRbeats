#include "include/IMUMaths.hpp"

#include <iostream>
#include <cmath>


namespace IMUMathsName{
    
    bool IMUMaths::QuadraticSum(float X, float Y, float Z){
        float QuadAccel = std::sqrt((X*X) + (Y*Y) + (Z*Z));


        // Play a sound if the acceleration is high enough
        if (QuadAccel > 15){
            return 1;
        } else {
            return 0;
        }
    }
    
}