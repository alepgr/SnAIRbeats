#include <gtest/gtest.h>
#include "../src/libs/ALSAPlayer/include/ALSAPlayer.hpp"

TEST(AudioTests, ConvertFiles){
    AudioPlayerName::AudioPlayer Audio("default",44100,2,SND_PCM_FORMAT_S16_LE,256,{"tests/test_data/SnareDrum.wav"});

    std::string key = "tests/test_data/SnareDrum.wav";
    EXPECT_NE(Audio.fileBuffers.find(key), Audio.fileBuffers.end());
    EXPECT_GT(Audio.fileBuffers[key].size(),0);

}

TEST(AuioTests, AddSoundToMixer){
    AudioPlayerName::AudioPlayer Audio("default",44100,2,SND_PCM_FORMAT_S16_LE,256,{"tests/test_data/SnareDrum.wav"});

    std::string key = "tests/test_data/SnareDrum.wav";
    ASSERT_NE(Audio.fileBuffers.find(key), Audio.fileBuffers.end());

    EXPECT_TRUE(Audio.addSoundToMixer(key));
}