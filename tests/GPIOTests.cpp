#include <gtest/gtest.h>
#include "IMUMaths.hpp"
#include "gpioevent.h"
#include "../src/libs/ALSAPlayer/include/ALSAPlayer.hpp"
#include <cmath>

TEST(GPIOTests, SetCallback) {
    // Create required dependencies
    AudioPlayerName::AudioPlayer audioPlayer;
    IMUMathsName::IMUMaths maths(audioPlayer);
    icm20948::ICM20948_I2C sensor(1, 0x69);

    // Create GPIOClass instance
    GPIOName::GPIOClass gpio("gpiochip0", 17, sensor, maths);

    class DummyCallback : public GPIOName::GPIOClass::Callback{
    public:
        void MathsCallback(float,float,float) override{

        }
    };

    DummyCallback cb; 

    // Initally no callback registered
    EXPECT_FALSE(gpio.HasCallback());

    //Register callback
    gpio.RegisterCallback(&cb);

    // After registration, see if callback is set
    EXPECT_TRUE(gpio.HasCallback());
    EXPECT_EQ(gpio.GetCallback(), &cb);
}

TEST(GPIOTests, GPIORunning) {
    AudioPlayerName::AudioPlayer audioPlayer;
    IMUMathsName::IMUMaths maths(audioPlayer);
    icm20948::ICM20948_I2C sensor(1, 0x69);

    GPIOName::GPIOClass gpio("gpiochip0", 17, sensor, maths);

    // Check that initially it's running
    EXPECT_TRUE(gpio.GetRunning());
    // std::cout << "TestGPIO"<<std::endl;

    // Call GPIOStop() to stop it
    gpio.GPIOStop();

    // Check that it's no longer running
    EXPECT_FALSE(gpio.GetRunning());

}
