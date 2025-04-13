#include "include/IMUMaths.hpp"
#include "../PlayAudio/include/PlayAudio.hpp"

#include <iostream>
#include <cmath>
#include <thread>


//How many samples to wait before another sound trigger
int delay = 100;
namespace IMUMathsName{

    //IMUMaths::IMUMaths(PlayAudioName::PlayAudio* audio) : audioPtr(audio) {}
    //Constructor to bring Audio object in and play sounds using it
    IMUMaths::IMUMaths(AudioPlayerName::AudioPlayer& Audio)
        :Audio(Audio), LastFilePlayed(0)
    {

    }

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
        if (!Pause){
            if (X <=-40 && X >=-45){
                //Play snare drum on X
                //std::thread soundThread(&PlayAudioName::PlayAudio::PlaySnare);
                std::thread soundThread([this]() {
                    this->Audio.playFile("src/libs/ALSAPlayer/include/SnareDrum.wav");
                });
                soundThread.detach();
                Pause = true;
                Counter = 0;
                LastFilePlayed = 1;
                //std::cout << LastFilePlayed << std::endl;
            } else if (Y <=-40 && Y >= -45){
                // Play high tom on Y
                //std::thread soundThread(&PlayAudioName::PlayAudio::PlayHighTom);
                std::thread soundThread([this]() {
                    this->Audio.playFile("src/libs/ALSAPlayer/include/HighTom.wav");
                });
                soundThread.detach();
                Pause = true;
                Counter = 0;
                LastFilePlayed = 2;
                //std::cout << LastFilePlayed << std::endl;
            } else if (Z <=20 && Z >= 15){
                //Play crash cymbal on Z
                //std::thread soundThread(&PlayAudioName::PlayAudio::PlayCymbal);
                std::thread soundThread([this]() {
                    this->Audio.playFile("src/libs/ALSAPlayer/include/CrashCymbal.wav");
                });
                soundThread.detach();
                Pause = true;
                Counter = 0;
                LastFilePlayed = 3;
                //std::cout << LastFilePlayed << std::endl;
            }
        } else if (Pause){
            Counter ++;
            if (Counter == delay){
                Pause = false;
            }
        } 
        //std::cout << LastFilePlayed << std::endl;
    }
    
}