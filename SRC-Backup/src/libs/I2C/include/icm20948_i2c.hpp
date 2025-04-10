#ifndef ICM20948_I2C_HPP
#define ICM20948_I2C_HPP

#include <cstdint>

#include "mraa/common.hpp"
#include "mraa/i2c.hpp"

#include "icm20948_defs.hpp"
#include "icm20948_utils.hpp"

namespace icm20948
{
    class ICM20948_I2C
    {
        private:
            mraa::I2c _i2c;
            unsigned _i2c_bus, _i2c_address;
            uint8_t _current_bank;
            float _accel_scale_factor, _gyro_scale_factor, _magn_scale_factor;

            bool _write_byte(const uint8_t bank, const uint8_t reg, const uint8_t byte);
            bool _read_byte(const uint8_t bank, const uint8_t reg, uint8_t &byte);
            bool _write_bit(const uint8_t bank, const uint8_t reg, const uint8_t bit_pos, const bool bit);
            bool _read_bit(const uint8_t bank, const uint8_t reg, const uint8_t bit_pos, bool &bit);
            bool _read_block_bytes(const uint8_t bank, const uint8_t start_reg, uint8_t *bytes, const int length);
            bool _write_mag_byte(const uint8_t mag_reg, const uint8_t byte);
            bool _read_mag_byte(const uint8_t mag_reg, uint8_t &byte);
            bool _read_int_byte(const uint8_t bank, const uint8_t reg, uint8_t &byte);

            bool _set_bank(uint8_t bank);
            bool _set_accel_sample_rate_div();
            bool _set_accel_range_dlpf();
            bool _set_gyro_sample_rate_div();
            bool _set_gyro_range_dlpf();
            
            bool _magnetometer_init();
            bool _magnetometer_enable();
            bool _magnetometer_set_mode();
            bool _magnetometer_configured();
            bool _magnetometer_set_readout();

            bool _chip_i2c_master_reset();

        public:
            // Contains linear acceleration in m/s^2
            float accel[3];
            // Contains angular velocities in rad/s
            float gyro[3];
            // Contains magnetic field strength in uTesla
            float magn[3];

            // Sensor settings
            icm20948::settings settings;

            // Constructor
            ICM20948_I2C(unsigned i2c_bus, unsigned i2c_address = ICM20948_I2C_ADDR, icm20948::settings 
                 = icm20948::settings());

            /**
             * @brief Initializes the ICM20948 sensor over I2C.
             *
             * This function performs the initialization sequence for the ICM20948 sensor.
             * It includes the following steps:
             * - Selects Bank 0 of the ICM20948 registers.
             * - Reads the WHO_AM_I register to verify the sensor's identity.
             * - Resets the sensor to ensure it is in a known state.
             * - Wakes up the sensor from sleep mode.
             * - Configures the sensor settings (e.g., accelerometer, gyroscope settings).
             * - Attempts to initialize the magnetometer up to three times.
             *
             * @return bool Returns true if the initialization sequence was successful, 
             *              including successful magnetometer initialization. Returns false otherwise.
             */
            bool init();

            /**
             * @brief Resets the ICM20948 sensor over I2C.
             *
             * This function issues a reset command to the ICM20948 sensor and waits until the reset process is complete.
             * It includes the following steps:
             * - Sets the reset bit in the PWR_MGMT_1 register to initiate a reset.
             * - Waits briefly (5 ms) to allow the reset to start.
             * - Polls the reset bit in the PWR_MGMT_1 register to check if the sensor is still resetting.
             * - Continues polling every 25 ms until the reset bit is cleared, indicating that the reset process is complete.
             * - Resets the internal bank tracking to Bank 0 after a successful reset.
             *
             * @return bool Returns true if the reset process was successful. Returns false if any step in the reset process fails.
             */
            bool reset();

            /**
             * @brief Wakes up the ICM20948 sensor from sleep mode over I2C.
             *
             * This function clears the sleep bit in the PWR_MGMT_1 register to wake the ICM20948 sensor from sleep mode.
             * It includes the following steps:
             * - Clears the sleep bit (bit 6) in the PWR_MGMT_1 register.
             * - Waits briefly (5 ms) to allow the sensor to stabilize after waking up.
             *
             * @return bool Returns true if the wake-up process was successful. Returns false if the operation fails.
             */
            bool wake();

            /**
             * @brief Configures the ICM20948 sensor settings over I2C.
             *
             * This function sets up various configuration parameters for the ICM20948 sensor, including:
             * - Accelerometer sample rate divider
             * - Accelerometer range and digital low-pass filter (DLPF) settings
             * - Gyroscope sample rate divider
             * - Gyroscope range and digital low-pass filter (DLPF) settings
             *
             * Each configuration step is performed by calling the respective private methods. The overall
             * success of the settings configuration is determined by the success of each individual step.
             *
             * @return bool Returns true if all settings were successfully applied. Returns false if any
             *              configuration step fails.
             */
            bool set_settings();

            /**
             * @brief Reads accelerometer and gyroscope data from the ICM20948 sensor over I2C.
             *
             * This function reads a block of 12 bytes from the ICM20948 sensor, which includes
             * the accelerometer and gyroscope data. It performs the following steps:
             * - Reads the accelerometer and gyroscope data from the sensor's registers.
             * - Reverses the byte order of the data for correct interpretation.
             * - Converts the raw accelerometer data to meters per second squared (m/s²) using the configured scale factor.
             * - Converts the raw gyroscope data to radians per second (rad/s) using the configured scale factor.
             * - Stores the processed accelerometer data in the `accel` array and gyroscope data in the `gyro` array.
             *
             * @return bool Returns true if the data was successfully read and processed. Returns false if the read operation fails.
             */
            bool read_accel_gyro();

            /**
             * @brief Reads magnetometer data from the ICM20948 sensor over I2C.
             *
             * This function reads a block of 6 bytes from the ICM20948 sensor, which contains the magnetometer data.
             * It performs the following steps:
             * - Reads the magnetometer data from the sensor's registers.
             * - Converts the raw magnetometer data to microteslas (µT) using the constant scale factor.
             * - Stores the processed magnetometer data in the `magn` array.
             *
             * @return bool Returns true if the magnetometer data was successfully read and processed. Returns false if the read operation fails.
             */
            bool read_magn();

            bool enable_DRDY_INT();

            bool check_DRDY_INT();
    };
}

#endif