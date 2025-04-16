#include "include/IMUMaths.hpp"
#include "../PlayAudio/include/PlayAudio.hpp"

#include <iostream>
#include <cmath>
#include <thread>


//How many samples to wait before another sound trigger
int delay = 40;
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
        SetPlayFileCallback([this](const std::string& FilePath){
            this->Audio.addSoundToMixer(FilePath);
        });
    }

    void IMUMaths::SetPlayFileCallback(const std::function<void(const std::string&)>& cb){
        PlayFileCallback = cb;
    }


    void IMUMaths::SoundChecker(float X, float Y, float Z){
        if (!Pause){
            if (X <=-60 && X >=-65){
                //Play snare drum on X
                LastFilePlayed = 1;
                //std::thread soundThread(&PlayAudioName::PlayAudio::PlaySnare);
                std::thread soundThread([this]() {
                    PlayFileCallback("src/libs/ALSAPlayer/include/SnareDrum.wav");
                });
                soundThread.detach();
                std::cout << "Snare" <<std::endl;
                Pause = true;
                Counter = 0;
                LastFilePlayed = 1;
                //std::cout << LastFilePlayed << std::endl;
            } else if (Y <=-60 && Y >= -65){
                // Play high tom on Y
                //std::thread soundThread(&PlayAudioName::PlayAudio::PlayHighTom);
                // std::thread soundThread([this]() {
                //     this->Audio.playFile("src/libs/ALSAPlayer/include/HighTom.wav");
                // });
                std::thread soundThread([this]() {
                    PlayFileCallback("src/libs/ALSAPlayer/include/HighTom.wav");
                });
                soundThread.detach();
                std::cout << "Tom" <<std::endl;
                Pause = true;
                Counter = 0;
                LastFilePlayed = 2;
                //std::cout << LastFilePlayed << std::endl;
            } else if (Z <=20 && Z >= 15){
                //Play crash cymbal on Z
                //std::thread soundThread(&PlayAudioName::PlayAudio::PlayCymbal);
                std::thread soundThread([this]() {
                    PlayFileCallback("src/libs/ALSAPlayer/include/CrashCymbal.wav");
                });
                soundThread.detach();
                std::cout << "Crash" <<std::endl;
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