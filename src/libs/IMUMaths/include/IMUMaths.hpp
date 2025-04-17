#ifndef IMUMATHS_H
#define IMUMATHS_H


#include <iostream> 
#include <array>
#include <iomanip>
#include <functional>

#include "../../ALSAPlayer/include/ALSAPlayer.hpp"


namespace IMUMathsName {
    class IMUMaths{
        private:
        
        std::function<void(const std::string&)> PlayFileCallback;

        public:
        AudioPlayerName::AudioPlayer &Audio;
        IMUMaths(AudioPlayerName::AudioPlayer &Audio);
        ~IMUMaths();   
        int LastFilePlayed;

        void SoundChecker(float X, float Y, float Z);
        
        void SetPlayFileCallback(const std::function<void(const std::string&)>& cb);




        bool Pause = false;
        int Counter = 0;


    };
}


#endif