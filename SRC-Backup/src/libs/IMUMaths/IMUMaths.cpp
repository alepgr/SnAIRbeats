#include "include/IMUMaths.hpp"
#include "../PlayAudio/include/PlayAudio.hpp"

#include <iostream>
#include <cmath>
#include <thread>

bool Pause = false;
int Counter = 0;

//How many samples to wait before another sound trigger
int delay = 100;
namespace IMUMathsName{

    //IMUMaths::IMUMaths(PlayAudioName::PlayAudio* audio) : audioPtr(audio) {}

    bool IMUMaths::QuadraticSum(float X, float Y, float Z){
        float QuadAccel = std::sqrt((X*X) + (Y*Y) + (Z*Z));


        // Play a sound if the acceleration is high enough
        if (QuadAccel > 15){
            return 1;
        } else {
            return 0;
        }
    }

    void IMUMaths::SoundChecker(float X, float Y, float Z){
        if (Pause == false){
            if (X <=-40 && X >=-45){
                //Play snare drum on X
                std::thread soundThread(&PlayAudioName::PlayAudio::PlaySnare);
                soundThread.detach();
                Pause = true;
                Counter = 0;
            } else if (Y <=-40 && Y >= -45){
                // Play high tom on Y
                std::thread soundThread(&PlayAudioName::PlayAudio::PlayHighTom);
                soundThread.detach();
                Pause = true;
                Counter = 0;
            } else if (Z <=20 && Z >= 15){
                //Play crash cymbal on Z
                std::thread soundThread(&PlayAudioName::PlayAudio::PlayCymbal);
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