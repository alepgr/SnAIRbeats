#include <gtest/gtest.h>
#include "IMUMaths.hpp"
#include "../src/libs/ALSAPlayer/include/ALSAPlayer.hpp"
#include <cmath>

class DummyAudioPlayer : public AudioPlayerName::AudioPlayer {
    };

TEST(MathsTests, TestSoundSnare) {
    DummyAudioPlayer audio;
    IMUMathsName::IMUMaths IMUmaths(audio);

    float x = -42.0f, y = 0.0f, z = 0.0f;
    IMUmaths.SoundChecker(x, y, z);
    EXPECT_EQ(IMUmaths.Pause, 1);
    EXPECT_EQ(IMUmaths.Counter, 0);
    EXPECT_EQ(IMUmaths.LastFilePlayed, 1);
}

TEST(MathsTests, TestSoundHighTom) {
    DummyAudioPlayer audio;
    IMUMathsName::IMUMaths IMUmaths(audio);

    float x = 0.0f, y = -42.0f, z = 0.0f;
    IMUmaths.SoundChecker(x, y, z);
    EXPECT_EQ(IMUmaths.Pause, 1);
    EXPECT_EQ(IMUmaths.Counter, 0);
    EXPECT_EQ(IMUmaths.LastFilePlayed, 2);
}

TEST(MathsTests, TestSoundCrash) {
    DummyAudioPlayer audio;
    IMUMathsName::IMUMaths IMUmaths(audio); 

    float x = 0.0f, y = 0.0f, z = 17.0f;
    IMUmaths.SoundChecker(x, y, z);
    EXPECT_EQ(IMUmaths.Pause, 1);
    EXPECT_EQ(IMUmaths.Counter, 0);
    EXPECT_EQ(IMUmaths.LastFilePlayed, 3);
}