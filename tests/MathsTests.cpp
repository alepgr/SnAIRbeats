#include <gtest/gtest.h>
#include "IMUMaths.hpp"
#include "../src/libs/ALSAPlayer/include/ALSAPlayer.hpp"
#include <cmath>

// Dummy Object needed to construct
class DummyAudioPlayer : public AudioPlayerName::AudioPlayer {
public:
    void playFile(const std::string& fileName) {
    }
};
// threshold set to 15
// TEST(MathsTests, TestQuadraticSumTrueAboveThreshold) {
//     DummyAudioPlayer audio; 
//     IMUMathsName::IMUMaths IMUmaths(audio);

//     // Set values so that sqrt(300) = 17.3, above threshold
//     float x = 10.0, y = 10.0, z = 10.0;
//     EXPECT_TRUE(IMUmaths.QuadraticSum(x, y, z));
// }

// TEST(MathsTests, TestQuadraticSumFalseBelowThreshold) {
//     DummyAudioPlayer audio;
//     IMUMathsName::IMUMaths IMUmaths(audio); 

//     // Set values so that sqrt(3) = 1.73, below threshold 
//     float x = 1.0, y = 1.0, z = 1.0;
//     EXPECT_FALSE(IMUmaths.QuadraticSum(x, y, z));
// }

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