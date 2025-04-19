#include <gtest/gtest.h>
#include "IMUMaths.hpp"
#include "../src/libs/ALSAPlayer/include/ALSAPlayer.hpp"
#include <cmath>

class DummyAudioPlayer : public AudioPlayerName::AudioPlayer {
    };


TEST(MathsTests, SetCallback){
    AudioPlayerName::AudioPlayer audio;
    IMUMathsName::IMUMaths IMUMaths(audio);

    class DummyCallback : public IMUMathsName::IMUMaths::Callback{
    public:
        void AudioTrigger(const std::string& FilePath) override{

        }

        ~DummyCallback() noexcept override = default;
    };

    DummyCallback cb;

    // Initally no callback registered
    EXPECT_FALSE(IMUMaths.HasCallback());
    
    //Register callback
    IMUMaths.RegisterCallback(&cb);

    // After registration, see if callback is set
    EXPECT_TRUE(IMUMaths.HasCallback());
    EXPECT_EQ(IMUMaths.GetCallback(), &cb);
}

TEST(MathsTests, TestSoundSnare) {
    AudioPlayerName::AudioPlayer audio;
    IMUMathsName::IMUMaths IMUmaths(audio);

    float x = -42.0f, y = 0.0f, z = 0.0f;
    IMUmaths.SoundChecker(x, y, z);
    EXPECT_EQ(IMUmaths.Pause, 1);
    EXPECT_EQ(IMUmaths.Counter, 0);
    EXPECT_EQ(IMUmaths.LastFilePlayed, 1);
}

TEST(MathsTests, TestSoundHighTom) {
    AudioPlayerName::AudioPlayer audio;
    IMUMathsName::IMUMaths IMUmaths(audio);

    float x = 0.0f, y = -42.0f, z = 0.0f;
    IMUmaths.SoundChecker(x, y, z);
    EXPECT_EQ(IMUmaths.Pause, 1);
    EXPECT_EQ(IMUmaths.Counter, 0);
    EXPECT_EQ(IMUmaths.LastFilePlayed, 2);
}

TEST(MathsTests, TestSoundCrash) {
    AudioPlayerName::AudioPlayer audio;
    IMUMathsName::IMUMaths IMUmaths(audio); 

    float x = 0.0f, y = 0.0f, z = 17.0f;
    IMUmaths.SoundChecker(x, y, z);
    EXPECT_EQ(IMUmaths.Pause, 1);
    EXPECT_EQ(IMUmaths.Counter, 0);
    EXPECT_EQ(IMUmaths.LastFilePlayed, 3);
}