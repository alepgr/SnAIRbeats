#ifndef PLAYAUDIO_H
#define PLAYAUDIO_H

#include <iostream>
#include <alsa/asoundlib.h>

namespace PlayAudioName{
    class PlayAudio{
        public:

        static void PlaySnare();
        static void PlayHighTom();
        static void PlayCymbal();
    };
}





#endif