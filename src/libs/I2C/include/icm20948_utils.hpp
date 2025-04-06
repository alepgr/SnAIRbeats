#ifndef ICM2094_UTILS_HPP
#define ICM2094_UTILS_HPP

#include <cstdint>
#include "yaml-cpp/yaml.h"

namespace icm20948
{
    /*** ACCELEROMETER SETTINGS TYPEDEFS ***/
    /**
     * @enum accel_scale
     * @brief Represents the accelerometer full-scale range settings.
     *
     * This enumeration defines the different full-scale ranges that can be selected 
     * for the accelerometer. The full-scale range determines the maximum measurable
     * acceleration by the sensor in terms of gravitational force (g).
     *
     * The possible values are:
     * - ACCEL_2G:  +/- 2g range
     * - ACCEL_4G:  +/- 4g range
     * - ACCEL_8G:  +/- 8g range
     * - ACCEL_16G: +/- 16g range
     */
    typedef enum {ACCEL_2G = 0,
                  ACCEL_4G,
                  ACCEL_8G,
                  ACCEL_16G
    } accel_scale;

    /**
     * @enum accel_dlpf_config
     * @brief Represents the accelerometer Digital Low Pass Filter (DLPF) configuration settings.
     *
     * This enumeration defines the different Digital Low Pass Filter (DLPF) configurations
     * that can be selected for the accelerometer. The DLPF is used to filter out high-frequency
     * noise from the sensor's measurements, with different cutoff frequencies available depending
     * on the application requirements.
     *
     * The possible values are:
     * - ACCEL_DLPF_246HZ: 246 Hz DLPF cutoff frequency
     * - ACCEL_DLPF_246HZ_2: Alternate 246 Hz DLPF cutoff frequency
     * - ACCEL_DLPF_111_4HZ: 111.4 Hz DLPF cutoff frequency
     * - ACCEL_DLPF_50_4HZ: 50.4 Hz DLPF cutoff frequency
     * - ACCEL_DLPF_23_9HZ: 23.9 Hz DLPF cutoff frequency
     * - ACCEL_DLPF_11_5HZ: 11.5 Hz DLPF cutoff frequency
     * - ACCEL_DLPF_5_7HZ: 5.7 Hz DLPF cutoff frequency
     * - ACCEL_DLPF_473HZ: 473 Hz DLPF cutoff frequency
     */
    typedef enum {ACCEL_DLPF_246HZ = 0, 
                  ACCEL_DLPF_246HZ_2,
                  ACCEL_DLPF_111_4HZ,
                  ACCEL_DLPF_50_4HZ,
                  ACCEL_DLPF_23_9HZ,
                  ACCEL_DLPF_11_5HZ,
                  ACCEL_DLPF_5_7HZ,
                  ACCEL_DLPF_473HZ
    } accel_dlpf_config;

    /**
     * @struct accel_settings
     * @brief Configuration settings for an accelerometer sensor.
     *
     * This structure defines the configuration parameters for the accelerometer, including
     * the sample rate, full-scale range, and digital low-pass filter (DLPF) settings.
     *
     * Members:
     * - uint16_t `sample_rate_div`: 
     *   The sample rate divider, which determines the inner sample data rate of the accelerometer. 
     *   The value should be in the range [0, 4095]. The formula for calculating the inner sample
     *   rate is: 1.125kHz / sample_rate_div
     *
     * - accel_scale `scale`: 
     *   The full-scale range of the accelerometer measurements. This defines the maximum 
     *   acceleration (in g) that the sensor can measure. The available options are defined 
     *   in the `accel_scale` enumeration.
     *
     * - bool `dlpf_enable`: 
     *   A flag indicating whether the Digital Low-Pass Filter (DLPF) is enabled. 
     *   If true, the DLPF is active and the cutoff frequency is determined by `dlpf_config`.
     *
     * - accel_dlpf_config `dlpf_config`: 
     *   The configuration settings for the DLPF, determining the filter's cutoff frequency. 
     *   The available options are defined in the `accel_dlpf_config` enumeration.
     *
     * Constructor:
     * - accel_settings(uint16_t sample_rate_div = 0,
     *                  accel_scale scale = ACCEL_2G,
     *                  bool dlpf_enable = true,
     *                  accel_dlpf_config dlpf_config = ACCEL_DLPF_246HZ)
     *   Initializes the structure with default values or specified parameters.
     */
    typedef struct accel_settings
    {
        // Range [0, 4095]
        uint16_t sample_rate_div;

        // Full scale of measurements +/-
        accel_scale scale;

        // Digital Low-Pass Filter enable
        bool dlpf_enable;

        // Digital Low-Pass Filter settings
        accel_dlpf_config dlpf_config;


        accel_settings(uint16_t sample_rate_div      = 0,
                       accel_scale scale             = ACCEL_2G,
                       bool dlpf_enable              = true,
                       accel_dlpf_config dlpf_config = ACCEL_DLPF_246HZ) : sample_rate_div(sample_rate_div),
                                                                           scale(scale),
                                                                           dlpf_enable(dlpf_enable),
                                                                           dlpf_config(dlpf_config) {};
    } accel_settings;



    /*** GYROSCOPE SETTINGS TYPEDEFS ***/
    /**
     * @enum gyro_scale
     * @brief Represents the gyroscope full-scale range settings.
     *
     * This enumeration defines the different full-scale ranges that can be selected 
     * for the gyroscope. The full-scale range determines the maximum measurable 
     * angular velocity by the sensor in degrees per second (DPS).
     *
     * The possible values are:
     * - GYRO_250DPS: 250 degrees per second full-scale range
     * - GYRO_500DPS: 500 degrees per second full-scale range
     * - GYRO_1000DPS: 1000 degrees per second full-scale range
     * - GYRO_2000DPS: 2000 degrees per second full-scale range
     */
    typedef enum {GYRO_250DPS = 0,
                  GYRO_500DPS, 
                  GYRO_1000DPS,
                  GYRO_2000DPS
    } gyro_scale;

    /**
     * @enum gyro_dlpf_config
     * @brief Represents the gyroscope Digital Low Pass Filter (DLPF) configuration settings.
     *
     * This enumeration defines the different Digital Low Pass Filter (DLPF) configurations 
     * that can be selected for the gyroscope. The DLPF is used to filter out high-frequency 
     * noise from the sensor's measurements, with various cutoff frequencies available  
     * depending on the desired noise reduction and signal fidelity.
     *
     * The possible values are:
     * - GYRO_DLPF_196_6HZ: 196.6 Hz DLPF cutoff frequency
     * - GYRO_DLPF_151_8HZ: 151.8 Hz DLPF cutoff frequency
     * - GYRO_DLPF_119_5HZ: 119.5 Hz DLPF cutoff frequency
     * - GYRO_DLPF_51_2HZ: 51.2 Hz DLPF cutoff frequency
     * - GYRO_DLPF_23_9HZ: 23.9 Hz DLPF cutoff frequency
     * - GYRO_DLPF_11_6HZ: 11.6 Hz DLPF cutoff frequency
     * - GYRO_DLPF_5_7HZ: 5.7 Hz DLPF cutoff frequency
     * - GYRO_DLPF_361_4HZ: 361.4 Hz DLPF cutoff frequency
     */
    typedef enum {GYRO_DLPF_196_6HZ = 0,
                  GYRO_DLPF_151_8HZ,
                  GYRO_DLPF_119_5HZ,
                  GYRO_DLPF_51_2HZ,
                  GYRO_DLPF_23_9HZ,
                  GYRO_DLPF_11_6HZ,
                  GYRO_DLPF_5_7HZ,
                  GYRO_DLPF_361_4HZ
    } gyro_dlpf_config;

    /**
     * @struct gyro_settings
     * @brief Configuration settings for a gyroscope sensor.
     *
     * This structure defines the configuration parameters for a gyroscope sensor, including 
     * the sample rate, full-scale range, and Digital Low-Pass Filter (DLPF) settings.
     *
     * Members:
     * - uint8_t sample_rate_div: 
     *   The sample rate divider, which determines the inner sample data rate of the gyroscope. 
     *   The value should be in the range [0, 250]. The formula for calculating the inner sample
     *   rate is: 1.1kHz / sample_rate_div
     *
     * - gyro_scale scale: 
     *   The full-scale range of the gyroscope measurements. This defines the maximum 
     *   angular velocity (in degrees per second) that the sensor can measure. The available 
     *   options are defined in the `gyro_scale` enumeration.
     *
     * - bool dlpf_enable: 
     *   A flag indicating whether the Digital Low-Pass Filter (DLPF) is enabled. 
     *   If true, the DLPF is active and the cutoff frequency is determined by `dlpf_config`.
     *
     * - gyro_dlpf_config dlpf_config: 
     *   The configuration settings for the DLPF, determining the filter's cutoff frequency. 
     *   The available options are defined in the `gyro_dlpf_config` enumeration.
     *
     * Constructor:
     * - gyro_settings(uint8_t sample_rate_div = 0,
     *                 gyro_scale scale = GYRO_250DPS,
     *                 bool dlpf_enable = true,
     *                 gyro_dlpf_config dlpf_config = GYRO_DLPF_196_6HZ)
     *   Initializes the structure with default values or specified parameters.
     */
    typedef struct gyro_settings
    {
        // Range [0, 250]
        uint8_t sample_rate_div;

        // Full scale of measurements +/-
        gyro_scale scale;

        // Digital Low-Pass Filter enable
        bool dlpf_enable;

        // Digital Low-Pass Filter settings
        gyro_dlpf_config dlpf_config;


        gyro_settings(uint8_t sample_rate_div    = 0,
                      gyro_scale scale             = GYRO_250DPS,
                      bool dlpf_enable             = true,
                      gyro_dlpf_config dlpf_config = GYRO_DLPF_196_6HZ) : sample_rate_div(sample_rate_div),
                                                                          scale(scale),
                                                                          dlpf_enable(dlpf_enable),
                                                                          dlpf_config(dlpf_config) {};
    } gyro_settings;



    /*** MAGNETOMETER SETTINGS TYPEDEFS ***/
    /**
     * @enum magn_mode
     * @brief Represents the magnetometer operating modes.
     *
     * This enumeration defines the different operating modes for the magnetometer, 
     * which control the sensor's measurement mode and update rate. Each mode  
     * specifies a different operational state or measurement frequency.
     *
     * The possible values are:
     * - MAGN_SHUTDOWN: Shutdown mode, which powers down the sensor to save energy.
     * - MAGN_SINGLE: Single measurement mode, where the sensor takes a single reading and then enters a low-power state.
     * - MAGN_10HZ: Measurement mode with a 10 Hz inner sample rate, providing updates at 10 times per second.
     * - MAGN_20HZ: Measurement mode with a 20 Hz inner sample rate, providing updates at 20 times per second.
     * - MAGN_50HZ: Measurement mode with a 50 Hz inner sample rate, providing updates at 50 times per second.
     * - MAGN_100HZ: Measurement mode with a 100 Hz inner sample rate, providing updates at 100 times per second.
     * - MAGN_SELF_TEST: Self-test mode, used for performing internal sensor diagnostics and calibration.
     */
    typedef enum {MAGN_SHUTDOWN = 0,
                  MAGN_SINGLE = 1, 
                  MAGN_10HZ = 2,
                  MAGN_20HZ = 4,
                  MAGN_50HZ = 6,
                  MAGN_100HZ = 8,
                  MAGN_SELF_TEST = 16
    } magn_mode;

    /**
     * @struct magn_settings
     * @brief Configuration settings for a magnetometer sensor.
     *
     * This structure defines the configuration parameters for a magnetometer sensor, 
     * including the operational mode. The mode determines how the sensor operates, 
     * including its measurement rate and diagnostic functions.
     *
     * Members:
     * - magn_mode mode: 
     *   The operation mode of the magnetometer sensor. This determines the sensor's 
     *   measurement frequency or operational state. The available options are defined 
     *   in the `magn_mode` enumeration.
     *
     * Constructor:
     * - magn_settings(magn_mode mode = MAGN_100HZ)
     *   Initializes the structure with the specified mode or a default mode of MAGN_100HZ.
     */
    typedef struct magn_settings
    {
        // Magnetometer operation mode
        magn_mode mode;

        magn_settings(magn_mode mode = MAGN_100HZ) : mode(mode) {};
    } magn_settings;



    /*** SENSOR SETTINGS TYPEDEFS ***/
    /**
     * @struct settings
     * @brief Aggregated configuration settings for sensor modules.
     *
     * This structure contains the configuration parameters for multiple sensor modules,
     * including accelerometer, gyroscope, and magnetometer settings. It allows for 
     * centralized management of all sensor settings in a single structure.
     *
     * Members:
     * - accel_settings `accel`: 
     *   Configuration settings for the accelerometer sensor, defining parameters such as 
     *   inner sample rate, full-scale range, and Digital Low-Pass Filter (DLPF) settings.
     *
     * - gyro_settings `gyro`: 
     *   Configuration settings for the gyroscope sensor, including inner sample rate,  
     *   full-scale range, and Digital Low-Pass Filter (DLPF) settings.
     *
     * - magn_settings `magn`: 
     *   Configuration settings for the magnetometer sensor, specifying the operation mode  
     *   and measurement frequency.
     *
     * Constructors:
     * - settings(accel_settings accel = accel_settings(), 
     *            gyro_settings gyro   = gyro_settings(),
     *            magn_settings magn   = magn_settings())
     *   Initializes the structure with specified or default settings for accelerometer, 
     *   gyroscope, and magnetometer.
     *
     * - settings(YAML::Node config_file_node)
     *   Initializes the structure with settings loaded from a YAML configuration file node.
     */
    typedef struct settings
    {
        accel_settings accel;
        gyro_settings  gyro;
        magn_settings  magn;


        settings(accel_settings accel = accel_settings(), 
                 gyro_settings gyro   = gyro_settings(),
                 magn_settings magn   = magn_settings()) : accel(accel), 
                                                           gyro(gyro),
                                                           magn(magn) {};

        /**
         * @brief Constructs a `settings` object from a YAML configuration node.
         *
         * This constructor initializes the `settings` structure by parsing a YAML configuration 
         * node. It sets up the accelerometer, gyroscope, and magnetometer settings based on the 
         * values provided in the YAML file.
         *
         * @param config_file_node A YAML node containing configuration data for the accelerometer, 
         *                         gyroscope, and magnetometer. The node should include sub-nodes 
         *                         for each sensor type with relevant settings such as sample rate, 
         *                         scale, and filter configurations.
         *
         * @throws std::runtime_error If an invalid mode is specified for the magnetometer in the 
         *                             YAML configuration.
         */
        settings(YAML::Node config_file_node);
    } settings;




    /*** METHODS ***/
    /**
     * @brief Calculates the scale factor for accelerometer measurements.
     *
     * This function determines the scale factor for the accelerometer based on the 
     * selected full-scale range. The scale factor is used to convert raw sensor readings 
     * into meaningful acceleration values in units of g (gravitational force).
     *
     * @param scale The full-scale range setting for the accelerometer, defined by the 
     *              `accel_scale` enumeration. Valid values include ACCEL_2G, ACCEL_4G, 
     *              ACCEL_8G, and ACCEL_16G.
     *
     * @return The scale factor (float) corresponding to the specified accelerometer
     *         full-scale range. The scale factor is expressed as the reciprocal (+/-)
     *         of the full-scale measurement range in terms of g.
     *
     * @throws std::invalid_argument If an invalid `accel_scale` value is provided.
     */
    float accel_scale_factor(accel_scale scale);

    /**
     * @brief Converts an accelerometer scale value to its corresponding string representation.
     *
     * This function maps an `accel_scale` enumeration value to a human-readable string that 
     * represents the accelerometer's full-scale range setting.
     *
     * @param scale The full-scale range setting for the accelerometer, defined by the 
     *              `accel_scale` enumeration. Valid values include ACCEL_2G, ACCEL_4G, 
     *              ACCEL_8G, and ACCEL_16G.
     *
     * @return A string representing the accelerometer scale. The possible return values 
     *         are "2G", "4G", "8G", and "16G". If an invalid `accel_scale` value is provided, 
     *         the function returns "<invalid accelerometer scale>".
     */
    std::string accel_scale_to_str(accel_scale scale);

    /**
     * @brief Converts an accelerometer Digital Low-Pass Filter (DLPF) configuration value to its corresponding string representation.
     *
     * This function maps an `accel_dlpf_config` enumeration value to a human-readable string 
     * that represents the accelerometer's DLPF cutoff frequency.
     *
     * @param config The Digital Low-Pass Filter configuration for the accelerometer, 
     *               defined by the `accel_dlpf_config` enumeration. Valid values include 
     *               ACCEL_DLPF_246HZ, ACCEL_DLPF_246HZ_2, ACCEL_DLPF_111_4HZ, 
     *               ACCEL_DLPF_50_4HZ, ACCEL_DLPF_23_9HZ, ACCEL_DLPF_11_5HZ, 
     *               ACCEL_DLPF_5_7HZ, and ACCEL_DLPF_473HZ.
     *
     * @return A string representing the accelerometer DLPF configuration. The possible return 
     *         values are "246Hz", "111.4Hz", "50.4Hz", "23.9Hz", "11.5Hz", "5.7Hz", and "473Hz". 
     *         If an invalid `accel_dlpf_config` value is provided, the function returns 
     *         "<invalid accelerometer DLPF config>".
     */
    std::string accel_dlpf_config_to_str(accel_dlpf_config config);


    /**
     * @brief Calculates the scale factor for gyroscope measurements.
     *
     * This function determines the scale factor for the gyroscope based on the selected 
     * full-scale range. The scale factor is used to convert raw sensor readings into 
     * meaningful angular velocity values in degrees per second (DPS).
     *
     * @param scale The full-scale range setting for the gyroscope, defined by the 
     *              `gyro_scale` enumeration. Valid values include GYRO_250DPS, 
     *              GYRO_500DPS, GYRO_1000DPS, and GYRO_2000DPS.
     *
     * @return The scale factor corresponding to the specified gyroscope full-scale 
     *         range. The scale factor is expressed as the reciprocal of the full-scale 
     *         measurement range in terms of DPS.
     *
     * @throws std::invalid_argument If an invalid `gyro_scale` value is provided.
     */
    float gyro_scale_factor(gyro_scale scale);

    /**
     * @brief Converts a gyroscope scale value to its corresponding string representation.
     *
     * This function maps a `gyro_scale` enumeration value to a human-readable string that 
     * represents the gyroscope's full-scale range setting.
     *
     * @param scale The full-scale range setting for the gyroscope, defined by the 
     *              `gyro_scale` enumeration. Valid values include GYRO_250DPS, 
     *              GYRO_500DPS, GYRO_1000DPS, and GYRO_2000DPS.
     *
     * @return A string representing the gyroscope scale. The possible return values 
     *         are "250DPS", "500DPS", "1000DPS", and "2000DPS". If an invalid `gyro_scale` 
     *         value is provided, the function returns "<invalid gyroscope scale>".
     */
    std::string gyro_scale_to_str(gyro_scale scale);

    /**
     * @brief Converts a gyroscope Digital Low-Pass Filter (DLPF) configuration value to its corresponding string representation.
     *
     * This function maps a `gyro_dlpf_config` enumeration value to a human-readable string 
     * that represents the gyroscope's DLPF cutoff frequency.
     *
     * @param config The Digital Low-Pass Filter configuration for the gyroscope, 
     *               defined by the `gyro_dlpf_config` enumeration. Valid values include 
     *               GYRO_DLPF_196_6HZ, GYRO_DLPF_151_8HZ, GYRO_DLPF_119_5HZ, 
     *               GYRO_DLPF_51_2HZ, GYRO_DLPF_23_9HZ, GYRO_DLPF_11_6HZ, 
     *               GYRO_DLPF_5_7HZ, and GYRO_DLPF_361_4HZ.
     *
     * @return A string representing the gyroscope DLPF configuration. The possible return 
     *         values are "196.6Hz", "151.8Hz", "119.5Hz", "51.2Hz", "23.9Hz", 
     *         "11.6Hz", "5.7Hz", and "361.4Hz". If an invalid `gyro_dlpf_config` 
     *         value is provided, the function returns "<invalid gyroscope DLPF config>".
     */
    std::string gyro_dlpf_config_to_str(gyro_dlpf_config config);


    /**
     * @brief Converts a magnetometer operation mode value to its corresponding string representation.
     *
     * This function maps a `magn_mode` enumeration value to a human-readable string 
     * that represents the magnetometer's operation mode or measurement frequency.
     *
     * @param mode The operation mode of the magnetometer, defined by the `magn_mode` enumeration. 
     *             Valid values include MAGN_SHUTDOWN, MAGN_SINGLE, MAGN_10HZ, MAGN_20HZ, 
     *             MAGN_50HZ, MAGN_100HZ, and MAGN_SELF_TEST.
     *
     * @return A string representing the magnetometer mode. The possible return values 
     *         are "Shutdown", "Single", "10Hz", "20Hz", "50Hz", "100Hz", and "Self-test". 
     *         If an invalid `magn_mode` value is provided, the function returns 
     *         "<invalid magnetometer mode>".
     */
    std::string magn_mode_to_str(magn_mode mode);
}

#endif