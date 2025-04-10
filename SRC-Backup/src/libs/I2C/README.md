# ICM20948 - I2C

This is a C++ library meant for communication using I2C with the IMU sensor ICM20948, like the one found in the popular [Sparkfun 9DoF IMU Breakout](https://www.sparkfun.com/products/15335). Suitable for use with Raspberry Pi and similar projects.

All the necessary registers and values have been taken from the sensor's [manual](https://invensense.tdk.com/wp-content/uploads/2016/06/DS-000189-ICM-20948-v1.3.pdf).

## Config

The library contains YAML parsing functionality to configure the sensor. Options include:
- Accelerometer settings
  - Measurement range
  - Inner sample rate (the frequency the inner microprocessor samples new values and stores them into its registers)
  - Digital Low-Pass Filter settings
- Gyroscope settings
  - Measurement range
  - Inner sample rate (the frequency the inner microprocessor samples new values and stores them into its registers)
  - Digital Low-Pass Filter settings
- Magnetometer settings
  - Inner sample rate (the frequency the inner microprocessor samples new values and stores them into its registers)

## Dependencies

- For parsing YAML files package uses [yaml-cpp](https://github.com/jbeder/yaml-cpp). It can be installed either following the steps on the repo link, or by running:
```bash
sudo apt-get install libyaml-cpp-dev
```

- Dependency for I2C communication is [mraa](https://github.com/eclipse/mraa). Installation instruction can be found on the repo link.

## Examples

Example files and usage can be found under **examples** folder. Just run `make` command inside it and executables will be generated.
