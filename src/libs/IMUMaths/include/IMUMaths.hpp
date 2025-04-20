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

        // Pauses
        bool Pause = false;

        // Counter variable
        int Counter = 0;

        /**
         * @brief Empty callback to later be filled. Includes destructor
         */
        struct Callback{
            virtual void AudioTrigger(const std::string& FilePath) = 0;
            virtual ~Callback(){};
        };

                
        /**
         * @brief Registers a callback
         * 
         * @param cb callback to register
         */
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

        int LastFilePlayedTest() const{
            return LastFilePlayed;
        }

        #endif


        private:
        
        Callback* callback = nullptr;
        std::function<void(const std::string&)> PlayFileCallback;

        // For debugging: Identifier of the last audio file played

        int LastFilePlayed;


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