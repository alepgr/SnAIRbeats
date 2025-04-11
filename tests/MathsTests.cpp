#include <gtest/gtest.h>
#include "IMUMaths.hpp"
#include <cmath>

// Dummy Object to construct the AudioPlayer
class DummyAudioPlayer : public AudioPlayerName::AudioPlayer {
public:
    // The method does nothing, just providing a basic implementation.
    void playFile(const std::string& fileName) {
        // Simulate the call, but don't play any actual audio
    }
};

// Now, writing the test cases
TEST(MathsTests, TestQuadraticSumTrueAboveThreshold) {
    DummyAudioPlayer audio;  // Use the dummy audio player
    IMUMathsName::IMUMaths maths(audio);  // Pass it to IMUMaths

    // Set values such that sqrt(300) = ~17.32, which is greater than 15
    float x = 10.0, y = 10.0, z = 10.0;
    EXPECT_TRUE(maths.QuadraticSum(x, y, z));  // Should return true
}

TEST(MathsTests, TestQuadraticSumFalseBelowThreshold) {
    DummyAudioPlayer audio;  // Use the dummy audio player
    IMUMathsName::IMUMaths maths(audio);  // Pass it to IMUMaths

    // Set values such that sqrt(3) = 1.73, which is less than 15
    float x = 1.0, y = 1.0, z = 1.0;
    EXPECT_FALSE(maths.QuadraticSum(x, y, z));  // Should return false
}
