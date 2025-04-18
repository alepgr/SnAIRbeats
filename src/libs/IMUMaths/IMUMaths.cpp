#include "include/IMUMaths.hpp"
#include "../ALSAPlayer/include/ALSAPlayer.hpp"

#include <iostream>
#include <cmath>
#include <thread>


//How many samples to wait before another sound trigger
int delay = 2;


namespace IMUMathsName{
    //IMUMaths::IMUMaths(PlayAudioName::PlayAudio* audio) : audioPtr(audio) {}
    //Constructor to bring Audio object in and play sounds using it
    IMUMaths::IMUMaths(AudioPlayerName::AudioPlayer& Audio)
        :Audio(Audio), LastFilePlayed(0)
    {
         if (!Audio.open()) {
             std::cerr << "[IMUMaths] Warning: Failed to open ALSA device from IMUMaths constructor.\n";
        }
        
        Audio.startMixer();
    }


    IMUMaths::~IMUMaths() {
        Audio.stopMixer();
        Audio.close();
    }


    void IMUMaths::SoundChecker(float X, float Y, float Z){
        if (!Pause){
            if (X <=-30){
                //Play snare drum on X
                if (callback){
                    callback -> AudioTrigger("src/libs/ALSAPlayer/include/SnareDrum.wav");
                }
                //std::cout << "Snare" <<std::endl;
                Pause = true;
                Counter = 0;
                LastFilePlayed = 1;
                //std::cout << LastFilePlayed << std::endl;
            } else if (Y <=-30){
                // Play high tom on Y
                if (callback){
                    callback -> AudioTrigger("src/libs/ALSAPlayer/include/HighTom.wav");
                }
                Pause = true;
                Counter = 0;
                LastFilePlayed = 2;
                //std::cout << LastFilePlayed << std::endl;
            } else if (Z >=12){
                //Play crash cymbal on Z
               if (callback){
                callback -> AudioTrigger("src/libs/ALSAPlayer/include/CrashCymbal.wav");
            }
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