#include <gtest/gtest.h>
#include "IMUMaths.hpp"
#include "gpioevent.h"
#include "../src/libs/ALSAPlayer/include/ALSAPlayer.hpp"
#include <cmath>


void DummyCallback(void* context, float x, float y, float z) {
}

TEST(GPIOTests, SetCallback) {
    // Create required dependencies
    AudioPlayerName::AudioPlayer audioPlayer;
    IMUMathsName::IMUMaths maths(audioPlayer);
    icm20948::ICM20948_I2C sensor(1, 0x69);

    // Create GPIOClass instance
    GPIOName::GPIOClass gpio("gpiochip0", 17, sensor, maths);

    // Simulate context that would be passed to callback function
    void* dummyContext = reinterpret_cast<void*>(0x1234);

    // Trigger Setcallback
    gpio.SetCallback(DummyCallback, dummyContext);

    // check the class member variables match the values passed in
    EXPECT_EQ(gpio.callback, DummyCallback);
    EXPECT_EQ(gpio.CallbackFunction, dummyContext);
}

TEST(GPIOTests, GPIOStop) {
    AudioPlayerName::AudioPlayer audioPlayer;
    IMUMathsName::IMUMaths maths(audioPlayer);
    icm20948::ICM20948_I2C sensor(1, 0x69);

    GPIOName::GPIOClass gpio("gpiochip0", 17, sensor, maths);

    // Check that initially it's running
    EXPECT_TRUE(gpio.IsRunning());

    // Call GPIOStop() to stop it
    gpio.GPIOStop();

    // Check that it's no longer running
    EXPECT_FALSE(gpio.IsRunning());
}
