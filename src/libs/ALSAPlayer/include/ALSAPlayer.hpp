
#ifndef ALSAPLAYER_H
#define ALSAPLAYER_H

#include <alsa/asoundlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include "AudioFile.h"
#include <unordered_map> 
#include <thread>    


namespace AudioPlayerName{
    class AudioPlayer{
        public:
        std::unordered_map<std::string, std::vector<int32_t>> fileBuffers;

        bool StopMixingThread = false;

        /** 
         * @brief Constructor for AudioPlayer class.
         * 
         * Handles audio file loading, conversion and playback.
         * 
         * @param device The name of the ALSA device to use.
         * @param rate Sample rate in Hz.
         * @param ch Number of channels.
         * @param fmt Format of audio data.
         * @param frames Number of frames per period.
         * @param filesToConvert Sound files used.
         */
        AudioPlayer(const std::string& device="default",
            unsigned int rate = 44100,
            unsigned int ch = 2,
            snd_pcm_format_t fmt = SND_PCM_FORMAT_S16_LE,
            snd_pcm_uframes_t frames = 256,
            const std::vector<std::string>& filesToConvert = {"src/libs/ALSAPlayer/include/CrashCymbal.wav",
                                                              "src/libs/ALSAPlayer/include/HighTom.wav",
                                                              "src/libs/ALSAPlayer/include/SnareDrum.wav"})
        : deviceName(device), sampleRate(rate), channels(ch),
        format(fmt), framesPerPeriod(frames), handle(nullptr)
        {
            if (!filesToConvert.empty()){
                ConvertFiles(filesToConvert);
            }
        }

        /** 
         * @brief Open PCM device for playback.
         * 
         * It includes the following steps: 
         * - open the PCM device
         * - allocate hardware parameters object and fill it in with default values
         * - set desired hardware parameters (set access type, format, number of channels, sample rate, period size)
         * - write parameters to the driver
         * - get period size
         * 
         * @return Returns true is open was successful, but false if something went wrong
         */
        bool open(){
            int rc = snd_pcm_open(&handle, deviceName.c_str(), SND_PCM_STREAM_PLAYBACK,0);
            if (rc < 0){
                std::cerr << "Unable to open PCM devices: " << snd_strerror(rc) << std::endl;
                return false;
            }

            snd_pcm_hw_params_t* params;
            snd_pcm_hw_params_alloca(&params);
            snd_pcm_hw_params_any(handle, params);
            snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
            snd_pcm_hw_params_set_format(handle, params, format);
            snd_pcm_hw_params_set_channels(handle, params, channels);

            unsigned int rate_near = sampleRate;
            snd_pcm_hw_params_set_rate_near(handle, params, &rate_near,0);
            
            rc = snd_pcm_hw_params_set_period_size_near(handle, params, &framesPerPeriod, 0);
            if (rc <0) {
                std::cerr << "Unable to set HW parameters: " << snd_strerror(rc) << std::endl;
                return false;
            }
            snd_pcm_uframes_t bufferSize = framesPerPeriod * 4;
            rc = snd_pcm_hw_params_set_buffer_size_near(handle, params, &bufferSize);
            if (rc < 0) {
                std::cerr << "Unable to set buffer size: " << snd_strerror(rc) << std::endl;
                return false;
            }

            rc = snd_pcm_hw_params(handle, params);
            if (rc < 0) {
             std::cerr << "Unable to set HW parameters: " << snd_strerror(rc) << std::endl;
             return false;
            }


            // Verify the final chosen period size and buffer size
            snd_pcm_hw_params_get_period_size(params, &framesPerPeriod, 0);
            snd_pcm_hw_params_get_buffer_size(params, &bufferSize);
            std::cout << "[DEBUG] Final period size: " << framesPerPeriod << std::endl;
            std::cout << "[DEBUG] Final buffer size: " << bufferSize << std::endl;
        
            return true;
        }


        /** 
         * @brief Start mixer thread
         * 
         * Ensures that a handle exists before opening the mixing thread and tells the user to open
         * the device of not.
         * Sets a boolean to false to control the thread and starts the mixer
         * 
         * @return Returns true if mixer thread started correctly, false if error
        */
        bool startMixer() {
            if (!handle) {
                std::cerr << "ALSA device is not open. Call open() first." << std::endl;
                return false;
            }
            StopMixingThread = false;
            mixThread = std::thread(&AudioPlayer::mixerThreadLoop, this);

            return true;
        }

        /**
         * @brief Stop mixer thread, closes thread when called
         */
        void stopMixer() {
            StopMixingThread = true;
            if (mixThread.joinable()) {
                mixThread.join();
            }
        }
        
        /** 
         * @brief Add input sound to mixer and play it.
         * 
         * It includes the following steps: 
         * - register detected sound in the mixer
         * - add sound to buffer and remove sounds that have finished playing
         * - play sound
         * 
         * @param fileKey Sound file key.
         * @return Returns true if sound was added to mixer, false if error
         */
        bool addSoundToMixer(const std::string& fileKey) {
            std::lock_guard<std::mutex> lock(ActiveMutex);

            // Check if file buffer exists
            auto it = fileBuffers.find(fileKey);
            if (it == fileBuffers.end()) {
                std::cerr << "Audio buffer not found for file: " << fileKey << std::endl;
                return false;
            }

            // Create a new ActiveSound
            ActiveSound newSound;
            
            // pointer to the file's buffer
            newSound.buffer = &it->second; 
            newSound.position = 0;

            // Add it to active sounds
            ActiveSounds.push_back(newSound);
            return true;
        }
        
        /** 
         * @brief Close PCM handle and free all associated resources.
         */
        void close() {
            stopMixer();
            if (handle) {
                snd_pcm_drop(handle);
                snd_pcm_close(handle);
                handle = nullptr;
            }
        }

        /** 
         * @brief Destructor 
         */
        ~AudioPlayer() {
            close();
        }


        private:
        std::string deviceName;
        unsigned int sampleRate;
        unsigned int channels;
        snd_pcm_format_t format;
        snd_pcm_uframes_t framesPerPeriod;
        snd_pcm_t* handle;

        std::thread mixThread;

        struct ActiveSound {
            std::vector<int32_t>* buffer; 
            size_t position;             
        };

        std::vector<ActiveSound> ActiveSounds;
        std::mutex ActiveMutex;

        void mixerThreadLoop() {
            // Allocate a buffer for one period of audio
            const size_t periodSizeSamples = framesPerPeriod * channels;
            std::vector<int32_t> mixBuffer(periodSizeSamples, 0);

            while (!StopMixingThread) {
                // Clear the mix buffer each iteration
                std::fill(mixBuffer.begin(), mixBuffer.end(), 0);

                {
                    // Locks the active list ofsounds
                    std::lock_guard<std::mutex> lock(ActiveMutex);

                    // Mixes all of the activesounds and removes those that have finished
                    for (auto it = ActiveSounds.begin(); it != ActiveSounds.end(); ) {
                        ActiveSound& sound = *it;
                        const size_t totalFrames = sound.buffer->size() / channels;
                        size_t framesLeft = totalFrames - sound.position;

                        // Callculate how may frakes are left to be mixed
                        size_t framesToMix = std::min<size_t>(framesPerPeriod, framesLeft);

                        //Mix the audio data from this sound into the buffer
                        for (size_t f = 0; f < framesToMix; ++f) {
                            for (unsigned int c = 0; c < channels; ++c) {
                                // Source index in the file buffer
                                size_t srcIndex = (sound.position + f) * channels + c;
                                // Destination index in the mix buffer
                                size_t dstIndex = f * channels + c;

                                //Add up the sample
                                mixBuffer[dstIndex] += (*sound.buffer)[srcIndex];
                            }
                        }

                        // Advance playback position
                        sound.position += framesToMix;

                        // If a sound has finished, remove it
                        if (sound.position >= totalFrames) {
                            it = ActiveSounds.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }

                // Write the mixed buffer to ALSA
                int rc = snd_pcm_writei(handle, mixBuffer.data(), framesPerPeriod);
                if (rc == -EPIPE) {
                    std::cerr << "Underrun occurred\n";
                    snd_pcm_prepare(handle);
                } else if (rc < 0) {
                    std::cerr << "Error from writei: " << snd_strerror(rc) << std::endl;
                }
            }
        }


        void ConvertFiles(const std::vector<std::string>& filePaths) {
            std::vector<int32_t> result;

            for (const auto& path : filePaths) {
                AudioFile<int32_t> file;
                if (!file.load(path)) {
                    std::cerr << "Error loading file: " << path << std::endl;
                    continue;
                }

                int fileChannels = file.getNumChannels();
                int ChannelSamples = file.getNumSamplesPerChannel();

                std::vector<int32_t> interleaved;
                interleaved.reserve(ChannelSamples * fileChannels);
                for (int i=0; i < ChannelSamples; ++i){
                    for (int ch = 0; ch < fileChannels; ++ch){
                        interleaved.push_back(file.samples[ch][i]);
                        
                    }
                    
                    
                }
                fileBuffers[path] = std::move(interleaved);

            }
        }


    };
}



#endif