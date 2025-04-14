#ifndef ALSAPLAYER_H
#define ALSAPLAYER_H

#include <alsa/asoundlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include "AudioFile.h"



namespace AudioPlayerName{
    class AudioPlayer{
        public:
        std::vector<int32_t> audioBuffer;
        std::unordered_map<std::string, std::vector<int32_t>> fileBuffers;

        bool StopMixingThread;
        bool CancelPlayback = true;


        AudioPlayer(const std::string& device="default",
            unsigned int rate = 44100,
            unsigned int ch = 2,
            snd_pcm_format_t fmt = SND_PCM_FORMAT_S16_LE,
            snd_pcm_uframes_t frames = 32,
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
            snd_pcm_hw_params_set_period_size_near(handle, params, &framesPerPeriod, 0);
            
            rc = snd_pcm_hw_params(handle, params);
            if (rc <0) {
                std::cerr << "Unable to set HW parameters: " << snd_strerror(rc) << std::endl;
                return false;
            }

            snd_pcm_hw_params_get_period_size(params, &framesPerPeriod,0);
            return true;
        }

        bool playFile(const std::string& fileKey) {
            
            CancelPlayback = true;

            if(handle) {
                snd_pcm_drop(handle);
                snd_pcm_prepare(handle);
            }

            CancelPlayback = false;

            if (!handle) {
                std::cerr << "Device not open. Call open() first.\n";
                return false;
            }
            if (fileBuffers.find(fileKey) == fileBuffers.end()) {
                std::cerr << "Audio buffer not found for file: " << fileKey << "\n";
                return false;
            }

            const std::vector<int32_t>& buffer = fileBuffers[fileKey];
            size_t totalFrames = buffer.size() / channels;
            size_t offset = 0;
            int rc = 0;

            if (CancelPlayback){
                std::cerr << "[DEBUG] Playback cancelled." << std::endl;
                return false;
            }


            while (offset < totalFrames) {
                snd_pcm_uframes_t framesToWrite = framesPerPeriod;
                if (offset + framesPerPeriod > totalFrames)
                    framesToWrite = totalFrames - offset;
                rc = snd_pcm_writei(handle, buffer.data() + offset * channels, framesToWrite);
                if (rc == -EPIPE) {
                    std::cerr << "Underrun occurred\n";
                    snd_pcm_prepare(handle);
                } else if (rc < 0) {
                    std::cerr << "Error from writei: " << snd_strerror(rc) << "\n";
                    return false;
                } else if (static_cast<snd_pcm_uframes_t>(rc) != framesToWrite) {
                    std::cerr << "Short write, wrote " << rc << " frames\n";
                } else {
                    offset += rc;
                }
            }
            snd_pcm_drain(handle);
            snd_pcm_prepare(handle);
            return true;
        }

        void close() {
            if (handle) {
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

        std::condition_variable MixCV;
        std::mutex MixCVMutex;

        struct ActiveSound {
            std::vector<int32_t>* buffer; 
            size_t position;             
        };

        std::vector<ActiveSound> ActiveSounds;
        std::mutex ActiveMutex;


        template<typename T>
        void printVector(const std::vector<T>& vec) {
        for (const auto& el : vec) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
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
                //printVector(interleaved);
                fileBuffers[path] = std::move(interleaved);

            }
        }


    };
}



#endif