#ifndef IMUMATHS_H
#define IMUMATHS_H


#include <iostream> 
#include <array>
#include <iomanip>
#include <functional>

#include "../../PlayAudio/include/PlayAudio.hpp"

#include "../../ALSAPlayer/include/ALSAPlayer.hpp"


namespace IMUMathsName {
    class IMUMaths{
        private:
        
        PlayAudioName::PlayAudio* audioPtr;
        std::function<void(const std::string&)> PlayFileCallback;

        public:
        AudioPlayerName::AudioPlayer &Audio;

        /**
         * @brief Constructs an object with access to the audio player
         * 
         * @param Audio used for playback
         */
        IMUMaths(AudioPlayerName::AudioPlayer &Audio);

        // For debugging: Identifier of the last audio file played
        int LastFilePlayed;

        /**
         * @brief It measures each axis and sees if it falls within desired thresholds
         * 
         * If the acceleration along the specified axis falls within specified thersholds,
         * it will play audio
         * 
         * @param X acceleration along the x-axis
         * @param Y acceleration along the Y-axis
         * @param Z acceleration along the Z-axis
         */
        void SoundChecker(float X, float Y, float Z);
        
        /**
         * @brief Sets the callback
         * 
         * It registers a callback via the function input
         * 
         * @param cb
         */
        void SetPlayFileCallback(const std::function<void(const std::string&)>& cb);

        // Pauses
        bool Pause = false;

        // Counter variable
        int Counter = 0;


    };
}


#endif