
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
        // std::vector<int32_t> audioBuffer;
        std::unordered_map<std::string, std::vector<int16_t>> fileBuffers;

        bool StopMixingThread = false;
        //  bool CancelPlayback = true;


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
            //Convert files to audio buffers here I think
            if (!filesToConvert.empty()){
                ConvertFiles(filesToConvert);
            }
        }

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
            
            // framesPerPeriod = 128;
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
            
            // // --------------------------------------------------------------------
            // // (3) Set software parameters (start threshold, etc.)
            // // --------------------------------------------------------------------
            // snd_pcm_sw_params_t* swparams;
            // snd_pcm_sw_params_alloca(&swparams);
            // snd_pcm_sw_params_current(handle, swparams);

            // // Make playback start when one period is in the buffer
            // // Instead of waiting for the buffer to fill halfway or fully
            // rc = snd_pcm_sw_params_set_start_threshold(handle, swparams, framesPerPeriod);
            // if (rc < 0) {
            //     std::cerr << "Unable to set start threshold: " << snd_strerror(rc) << std::endl;
            //     return false;
            // }

            // // Make ALSA consider data “available” once we have one period's worth
            // rc = snd_pcm_sw_params_set_avail_min(handle, swparams, framesPerPeriod);
            // if (rc < 0) {
            //     std::cerr << "Unable to set avail min: " << snd_strerror(rc) << std::endl;
            //     return false;
            // }

            // rc = snd_pcm_sw_params(handle, swparams);
            // if (rc < 0) {
            //     std::cerr << "Unable to set software parameters: " << snd_strerror(rc) << std::endl;
            //     return false;
            // }


            //snd_pcm_hw_params_get_period_size(params, &framesPerPeriod,0);
            return true;
        }


        void startMixer() {
            if (!handle) {
                std::cerr << "ALSA device is not open. Call open() first." << std::endl;
                return;
            }
            StopMixingThread = false;
            mixThread = std::thread(&AudioPlayer::mixerThreadLoop, this);
        }


        void stopMixer() {
            StopMixingThread = true;
            if (mixThread.joinable()) {
                mixThread.join();
            }
        }
        

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
            newSound.buffer = &it->second;  // pointer to the file's buffer
            newSound.position = 0;

            // Add it to active sounds
            ActiveSounds.push_back(newSound);
            return true;
        }


        // bool playFile(const std::string& fileKey) {
            
        //     CancelPlayback = true;

        //     if(handle) {
        //         snd_pcm_drop(handle);
        //         snd_pcm_prepare(handle);
        //     }

        //     CancelPlayback = false;

        //     if (!handle) {
        //         std::cerr << "Device not open. Call open() first.\n";
        //         return false;
        //     }
        //     if (fileBuffers.find(fileKey) == fileBuffers.end()) {
        //         std::cerr << "Audio buffer not found for file: " << fileKey << "\n";
        //         return false;
        //     }

        //     const std::vector<int32_t>& buffer = fileBuffers[fileKey];
        //     size_t totalFrames = buffer.size() / channels;
        //     size_t offset = 0;
        //     int rc = 0;

        //     if (CancelPlayback){
        //         std::cerr << "[DEBUG] Playback cancelled." << std::endl;
        //         return false;
        //     }


        //     while (offset < totalFrames) {
        //         snd_pcm_uframes_t framesToWrite = framesPerPeriod;
        //         if (offset + framesPerPeriod > totalFrames)
        //             framesToWrite = totalFrames - offset;
        //         rc = snd_pcm_writei(handle, buffer.data() + offset * channels, framesToWrite);
        //         if (rc == -EPIPE) {
        //             std::cerr << "Underrun occurred\n";
        //             snd_pcm_prepare(handle);
        //         } else if (rc < 0) {
        //             std::cerr << "Error from writei: " << snd_strerror(rc) << "\n";
        //             return false;
        //         } else if (static_cast<snd_pcm_uframes_t>(rc) != framesToWrite) {
        //             std::cerr << "Short write, wrote " << rc << " frames\n";
        //         } else {
        //             offset += rc;
        //         }
        //     }
        //     snd_pcm_drain(handle);
        //     snd_pcm_prepare(handle);
        //     return true;
        // }

        void close() {
            stopMixer();
            if (handle) {
                snd_pcm_drop(handle);
                snd_pcm_close(handle);
                handle = nullptr;
            }
        }

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

        // std::condition_variable MixCV;
        // std::mutex MixCVMutex;

        struct ActiveSound {
            std::vector<int16_t>* buffer; 
            size_t position;             
        };

        std::vector<ActiveSound> ActiveSounds;
        std::mutex ActiveMutex;


        // template<typename T>
        // void printVector(const std::vector<T>& vec) {
        // for (const auto& el : vec) {
        //     std::cout << el << " ";
        // }
        // std::cout << std::endl;
        // }

        void mixerThreadLoop() {
            // Allocate a buffer for one period of audio
            const size_t periodSizeSamples = framesPerPeriod * channels;
            std::vector<int16_t> mixBuffer(periodSizeSamples, 0);

            while (!StopMixingThread) {
                // 1) Clear the mix buffer to 0 each iteration
                std::fill(mixBuffer.begin(), mixBuffer.end(), 0);

                {
                    // 2) Lock the active sounds list
                    std::lock_guard<std::mutex> lock(ActiveMutex);

                    // We'll iterate over all active sounds, mixing them in.
                    // We also remove any that are finished playing.
                    for (auto it = ActiveSounds.begin(); it != ActiveSounds.end(); ) {
                        ActiveSound& sound = *it;
                        const size_t totalFrames = sound.buffer->size() / channels;
                        size_t framesLeft = totalFrames - sound.position;

                        // How many frames can we mix this period?
                        size_t framesToMix = std::min<size_t>(framesPerPeriod, framesLeft);

                        // 3) Mix the audio data from this sound into mixBuffer
                        for (size_t f = 0; f < framesToMix; ++f) {
                            for (unsigned int c = 0; c < channels; ++c) {
                                // Source index in the file buffer
                                size_t srcIndex = (sound.position + f) * channels + c;
                                // Destination index in the mix buffer
                                size_t dstIndex = f * channels + c;

                                // Accumulate (sum) the sample
                                mixBuffer[dstIndex] += (*sound.buffer)[srcIndex];
                            }
                        }

                        // Advance this sound’s playback position
                        sound.position += framesToMix;

                        // If we've reached the end of the sound, remove it
                        if (sound.position >= totalFrames) {
                            it = ActiveSounds.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }

                // 4) Write the mixed buffer to ALSA
                // In a real system, you might want to handle partial writes, etc.
                int rc = snd_pcm_writei(handle, mixBuffer.data(), framesPerPeriod);
                if (rc == -EPIPE) {
                    std::cerr << "Underrun occurred\n";
                    snd_pcm_prepare(handle);
                } else if (rc < 0) {
                    std::cerr << "Error from writei: " << snd_strerror(rc) << std::endl;
                    // Depending on your needs, you might break or keep going
                }

                // 5) (Optional) Sleep or yield if desired
                //    Usually ALSA blocks until framesPerPeriod are accepted,
                //    so an explicit sleep might not be necessary. But you
                //    might do std::this_thread::sleep_for(...) if you wish.
            }
        }


        void ConvertFiles(const std::vector<std::string>& filePaths) {
            std::vector<int16_t> result;

            for (const auto& path : filePaths) {
                AudioFile<int16_t> file;
                if (!file.load(path)) {
                    std::cerr << "Error loading file: " << path << std::endl;
                    continue;
                }

                int fileChannels = file.getNumChannels();
                int ChannelSamples = file.getNumSamplesPerChannel();

                std::vector<int16_t> interleaved;
                interleaved.reserve(ChannelSamples * fileChannels);
                for (int i=0; i < ChannelSamples; ++i){
                    for (int ch = 0; ch < fileChannels; ++ch){
                        interleaved.push_back(file.samples[ch][i]);
                        
                    }
                    
                    
                }
                //printVector(interleaved);
                fileBuffers[path] = std::move(interleaved);

            }
        }


    };
}



#endif
