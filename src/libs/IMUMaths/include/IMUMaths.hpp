#ifndef IMUMATHS_H
#define IMUMATHS_H


#include <iostream> 
#include <array>
#include <iomanip>
#include <functional>

#include "../../ALSAPlayer/include/ALSAPlayer.hpp"


namespace IMUMathsName {
    class IMUMaths{
        
        public:
        AudioPlayerName::AudioPlayer &Audio;

        /**
         * @brief Constructs an object with access to the audio player
         * 
         * @param Audio used for playback
         */
        IMUMaths(AudioPlayerName::AudioPlayer &Audio);

        ~IMUMaths();   


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

        struct Callback{
            virtual void AudioTrigger(const std::string& FilePath) = 0;
            virtual ~Callback(){};
        };

        void RegisterCallback(Callback* cb){
            callback = cb;
            //std::cout << "[IMUMaths] Registered callback at address: " << callback << std::endl;
        }

        //Access to private fo UNIT_TEST only
        #ifdef UNIT_TEST 
        
        bool HasCallback() const {
            return callback != nullptr;
        }

        Callback* GetCallback() const {
            return callback;
        }

        #endif


        private:
        
        Callback* callback = nullptr;
        std::function<void(const std::string&)> PlayFileCallback;


    };

    struct AudioCallback : IMUMathsName::IMUMaths::Callback{
        AudioPlayerName::AudioPlayer& Audio;

        AudioCallback(AudioPlayerName::AudioPlayer& audio) : Audio(audio) {}

        virtual void AudioTrigger(const std::string& FilePath){
            std::thread([this,FilePath]{
                Audio.addSoundToMixer(FilePath);
                
            }).detach();
        }
    };
}


#endif