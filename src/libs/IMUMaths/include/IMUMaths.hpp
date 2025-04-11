#ifndef IMUMATHS_H
#define IMUMATHS_H


#include <iostream> 
#include <array>
#include <iomanip>

#include "../../PlayAudio/include/PlayAudio.hpp"

#include "../../ALSAPlayer/include/ALSAPlayer.hpp"


namespace IMUMathsName {
    class IMUMaths{
        private:
        
        PlayAudioName::PlayAudio* audioPtr;

        public:
        AudioPlayerName::AudioPlayer &Audio;
        IMUMaths(AudioPlayerName::AudioPlayer &Audio);
        //explicit IMUMaths(PlayAudioName::PlayAudio* audio) : audioPtr(audio);
        bool QuadraticSum(float X, float Y, float Z);
        std::array<float, 10>bufferX;
        std::array<float, 10>bufferY;
        std::array<float, 10>bufferZ;

        void SoundChecker(float X, float Y, float Z);
        

        bool Pause = true;
        int Counter = true;


    };
}


#endif