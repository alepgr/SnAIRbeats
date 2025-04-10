#include "include/AudioLib.hpp"
#include <alsa/asoundlib.h>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <cstring>
#include <vector>


namespace AudioLib {

    #pragma pack(push, 1)
    struct WAVHeader {
        char riff[4];            
        uint32_t chunkSize;      
        char wave[4];            
        char fmt[4];             
        uint32_t subchunk1Size;  
        uint16_t audioFormat;    
        uint16_t numChannels;    
        uint32_t sampleRate;     
        uint32_t byteRate;       
        uint16_t blockAlign;     
        uint16_t bitsPerSample;  
        char data[4];            
        uint32_t dataSize;       
    };
    #pragma pack(pop)

    AudioLib::AudioLib(const std::string &device){
        int err = snd_pcm_open(&pcmHandle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
        if (err < 0) {
            throw std::runtime_error("[ERROR] Can't open audio device - womp womp: " +
            std::string(snd_strerror(err)));
        }

        snd_pcm_hw_params_t *params;
        snd_pcm_hw_params_malloc(&params);
        snd_pcm_hw_params_any(pcmHandle, params);
    
        err = snd_pcm_hw_params_set_access(pcmHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
        if (err < 0){
            snd_pcm_hw_params_free(params);
            throw std::runtime_error("[ERROR] Setting access type: " + std::string(snd_strerror(err)));
        }

        err = snd_pcm_hw_params_set_format(pcmHandle, params, SND_PCM_FORMAT_S16_LE);
        if (err < 0) {
            snd_pcm_hw_params_free(params);
            throw std::runtime_error("Error setting format: " + std::string(snd_strerror(err)));
        }

        unsigned int sampleRate = 44100;
        err = snd_pcm_hw_params_set_rate_near(pcmHandle, params, &sampleRate, 0);
        if (err < 0) {
            snd_pcm_hw_params_free(params);
            throw std::runtime_error("Error setting channels: " + std::string(snd_strerror(err)));
        }

        unsigned int channels = 1;
        err = snd_pcm_hw_params_set_channels(pcmHandle, params, channels);
        if (err < 0){
            snd_pcm_hw_params_free(params);
            throw std::runtime_error("Error setting channels: " + std::string(snd_strerror(err)));
        }


        err = snd_pcm_hw_params(pcmHandle,params);
        snd_pcm_hw_params_free(params);
        if (err < 0){
            throw std::runtime_error("Error setting HW params: " + std::string(snd_strerror(err)));
            }
        std::cout << "Audio device opened and configured successfully!" << std::endl;
        }
    

    AudioLib::~AudioLib(){
        if (pcmHandle) {
            snd_pcm_close(pcmHandle);
        }
    }

    void AudioLib::AudioLib::PlaySound(){
            const double frequency = 440.0;
            const unsigned int sampleRate = 44100;
            const double duration = 1.0;
            const int channels = 2;
            const size_t numFrames = static_cast<size_t>(duration*sampleRate);
            const size_t bufferSize = numFrames * channels;

            short *buffer = new short[bufferSize];

            //Generate Sine wave
            for (size_t i=0; i<numFrames; i++){
                double sample = sin((2.0 * M_PI * frequency * i) / sampleRate);
                short value= static_cast<short>(sample * 32767);
                
                //Both channels
                buffer[i * channels] = value;
                buffer[i * channels + 1] = value;
            }

            size_t framesRemaining = numFrames;
            while (framesRemaining > 0){
                int err = snd_pcm_writei(pcmHandle, buffer + (numFrames - framesRemaining) * channels, framesRemaining);

                if (err < 0) { 
                    snd_pcm_prepare(pcmHandle);
                } else {
                    framesRemaining -= err;
                }
            }

            delete[] buffer;
    }


    void AudioLib::AudioLib::PlayFile() {
        const std::string filename = "src/libs/ALSAAudio/AudioFiles/SnareDrum1.wav";
        std::ifstream wavFile(filename, std::ios::binary);
        if (!wavFile) {
            std::cerr << "Cannot open the file: " << filename << std::endl;
            return;
        }

        WAVHeader header;
        wavFile.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));
        if (!wavFile){
            std::cerr << "Failed to read Wave Header - womp womp" << std::endl;
            return;
        }

        if (std::strncmp(header.riff, "RIFF", 4) != 0 || std::strncmp(header.wave, "WAVE", 4) != 0) {
            std::cerr << "Invalid WAV file format dumb dumb." << std::endl;
            return;
        }

        // For debugging, print the header info.
        std::cout << "Playing WAV file: " << filename << std::endl;
        std::cout << "Sample Rate: " << header.sampleRate << std::endl;
        std::cout << "Channels: " << header.numChannels << std::endl;
        std::cout << "Bits per sample: " << header.bitsPerSample << std::endl;
        std::cout << "Data size: " << header.dataSize << " bytes" << std::endl;

        std::vector<char> pcmData(header.dataSize);
        wavFile.read(pcmData.data(), header.dataSize);
        if (!wavFile) {
            std::cerr << "Failed to read PCM data." << std::endl;
            return;
    }

    //Calculate the number of frames.
    //A frame is one sample per channel
    size_t frameSize = header.numChannels * (header.bitsPerSample/8);
    size_t numFrames= header.dataSize /frameSize;

    //Write PCM data to ALSA device
    size_t framesWritten =0;
    const char* dataPtr = pcmData.data();
    while (framesWritten < numFrames ){
        int err = snd_pcm_writei(pcmHandle, dataPtr + framesWritten * frameSize, numFrames - framesWritten);

        if (err < 0) {
            err = snd_pcm_recover(pcmHandle, err, 0);
            if (err < 0) {
                std::cerr << "snd_pcm_writei failed: " << snd_strerror(err) << std::endl;
                break;
            }
        }
        framesWritten += err;        
    }

    int drainErr= snd_pcm_drain(pcmHandle);
    if(drainErr < 0){
        std::cerr << "Drain error: " << snd_strerror(drainErr) << std::endl;
    }
}

    void AudioLib::PlayAudioTerminal(){
        //Change to be switch case and play depending on position of sticks
        int ret = system("aplay -D plughw:2,0 src/libs/ALSAAudio/AudioFiles/SnareDrum1.wav");

    }

}
