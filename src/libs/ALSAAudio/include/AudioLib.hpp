#ifndef AUDIOLIB_H
#define AUDIOLIB_H

#include <alsa/asoundlib.h>
#include <iostream>

namespace AudioLib {
    class AudioLib {
    private:
        snd_pcm_t *pcmHandle= nullptr;

    public:
    AudioLib(const std::string &device = "default");
    ~AudioLib();

    void PlaySound();
    void PlayFile();
    void PlayAudioTerminal();
    };
}





#endif