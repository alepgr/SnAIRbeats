#include "include/PlayAudio.hpp"
#include <alsa/asoundlib.h>
#include <iostream>


namespace PlayAudioName{
    void PlayAudio::PlaySnare(){
        int ret = system("aplay -D pulse src/libs/PlayAudio/include/SnareDrum.wav");

    }

    void PlayAudio::PlayHighTom(){
        int ret = system("aplay -D pulse src/libs/PlayAudio/include/HighTom.wav");

    }
    
    void PlayAudio::PlayCymbal(){
        int ret = system("aplay -D pulse src/libs/PlayAudio/include/CrashCymbal.wav");

    }
}