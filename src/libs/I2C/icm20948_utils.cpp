#include "icm20948_utils.hpp"

#include <stdexcept>
#include <string>

namespace icm20948
{
    float accel_scale_factor(accel_scale scale)
    {
        switch(scale)
        {
            case ACCEL_2G:
                return 1/16384.0f;
                break;
            case ACCEL_4G:
                return 1/8192.0f;
                break;
            case ACCEL_8G:
                return 1/4096.0f;
                break;
            case ACCEL_16G:
                return 1/2048.0f;
                break;
            default:
                throw(std::invalid_argument("Invalid argument in accel_scale_factor(): " + std::to_string(scale)));
        }
    }

    std::string accel_scale_to_str(accel_scale scale)
    {
        switch(scale)
        {
            case ACCEL_2G:
                return "2G";
                break;
            case ACCEL_4G:
                return "4G";
                break;
            case ACCEL_8G:
                return "8G";
                break;
            case ACCEL_16G:
                return "16G";
                break;
            default:
                return "<invalid accelerometer scale>";
        }
    }

    std::string accel_dlpf_config_to_str(accel_dlpf_config config)
    {
        switch(config)
        {
            case ACCEL_DLPF_246HZ:
                return "246Hz";
                break;
            case ACCEL_DLPF_246HZ_2:
                return "246Hz";
                break;
            case ACCEL_DLPF_111_4HZ:
                return "111.4Hz";
                break;
            case ACCEL_DLPF_50_4HZ:
                return "50.4Hz";
                break;
            case ACCEL_DLPF_23_9HZ:
                return "23.9Hz";
                break;
            case ACCEL_DLPF_11_5HZ:
                return "11.5Hz";
                break;
            case ACCEL_DLPF_5_7HZ:
                return "5.7Hz";
                break;
            case ACCEL_DLPF_473HZ:
                return "473Hz";
                break;
            default:
                return "<invalid accelerometer DLPF config>";
        }
    }

    
    float gyro_scale_factor(gyro_scale scale)
    {
        switch(scale)
        {
            case GYRO_250DPS:
                return 1/131.0f;
                break;
            case GYRO_500DPS:
                return 1/65.5f;
                break;
            case GYRO_1000DPS:
                return 1/32.8f;
                break;
            case GYRO_2000DPS:
                return 1/16.4f;
                break;
            default:
                throw(std::invalid_argument("Invalid argument in accel_scale_factor(): " + std::to_string(scale)));
        }
    }

    std::string gyro_scale_to_str(gyro_scale scale)
    {
        switch(scale)
        {
            case GYRO_250DPS:
                return "250DPS";
                break;
            case GYRO_500DPS:
                return "500DPS";
                break;
            case GYRO_1000DPS:
                return "1000DPS";
                break;
            case GYRO_2000DPS:
                return "2000DPS";
                break;
            default:
                return "<invalid gyroscope scale>";
        }
    }

    std::string gyro_dlpf_config_to_str(gyro_dlpf_config config)
    {
        switch(config)
        {
            case GYRO_DLPF_196_6HZ:
                return "196.6Hz";
                break;
            case GYRO_DLPF_151_8HZ:
                return "151.8Hz";
                break;
            case GYRO_DLPF_119_5HZ:
                return "119.5Hz";
                break;
            case GYRO_DLPF_51_2HZ:
                return "51.2Hz";
                break;
            case GYRO_DLPF_23_9HZ:
                return "23.9Hz";
                break;
            case GYRO_DLPF_11_6HZ:
                return "11.6Hz";
                break;
            case GYRO_DLPF_5_7HZ:
                return "5.7Hz";
                break;
            case GYRO_DLPF_361_4HZ:
                return "361.4Hz";
                break;
            default:
                return "<invalid gyroscope DLPF config>";
        }
    }


    std::string magn_mode_to_str(magn_mode mode)
    {
        switch(mode)
        {
            case MAGN_SHUTDOWN:
                return "Shutdown";
                break;
            case MAGN_SINGLE:
                return "Single";
                break;
            case MAGN_10HZ:
                return "10Hz";
                break;
            case MAGN_20HZ:
                return "20Hz";
                break;
            case MAGN_50HZ:
                return "50Hz";
                break;
            case MAGN_100HZ:
                return "100Hz";
                break;
            case MAGN_SELF_TEST:
                return "Self-test";
                break;
            default:
                return "<invalid magnetometer mode>";
        }
    }


    settings::settings(YAML::Node config_file_node)
    {
        for(YAML::const_iterator it = config_file_node.begin(); it != config_file_node.end(); ++it)
        {
            if(it->first.as<std::string>() == "accelerometer")
            {
                for(YAML::const_iterator accel_it = it->second.begin(); accel_it != it->second.end(); ++accel_it)
                {
                    if(accel_it->first.as<std::string>() == "sample_rate_div")
                    {
                        this->accel.sample_rate_div = (uint16_t)accel_it->second.as<unsigned>();
                    }
                    else if(accel_it->first.as<std::string>() == "scale")
                    {
                        this->accel.scale = (accel_scale)accel_it->second.as<unsigned>();
                    }
                    else if(accel_it->first.as<std::string>() == "dlpf")
                    {
                        for(YAML::const_iterator accel_dlpf_it = accel_it->second.begin(); accel_dlpf_it != accel_it->second.end(); ++accel_dlpf_it)
                        {
                            if(accel_dlpf_it->first.as<std::string>() == "enable")
                            {
                                this->accel.dlpf_enable = (bool)accel_dlpf_it->second.as<int>();
                            }
                            else if(accel_dlpf_it->first.as<std::string>() == "cutoff")
                            {
                                this->accel.dlpf_config = (accel_dlpf_config)accel_dlpf_it->second.as<unsigned>();
                            }
                        }
                    }
                }
            }

            if(it->first.as<std::string>() == "gyroscope")
            {
                for(YAML::const_iterator gyro_it = it->second.begin(); gyro_it != it->second.end(); ++gyro_it)
                {
                    if(gyro_it->first.as<std::string>() == "sample_rate_div")
                    {
                        this->gyro.sample_rate_div = (uint8_t)gyro_it->second.as<unsigned>();
                    }
                    else if(gyro_it->first.as<std::string>() == "scale")
                    {
                        this->gyro.scale = (gyro_scale)gyro_it->second.as<unsigned>();
                    }
                    else if(gyro_it->first.as<std::string>() == "dlpf")
                    {
                        for(YAML::const_iterator gyro_dlpf_it = gyro_it->second.begin(); gyro_dlpf_it != gyro_it->second.end(); ++gyro_dlpf_it)
                        {
                            if(gyro_dlpf_it->first.as<std::string>() == "enable")
                            {
                                this->gyro.dlpf_enable = (bool)gyro_dlpf_it->second.as<int>();
                            }
                            else if(gyro_dlpf_it->first.as<std::string>() == "cutoff")
                            {
                                this->gyro.dlpf_config = (gyro_dlpf_config)gyro_dlpf_it->second.as<unsigned>();
                            }
                        }
                    }
                }
            }

            if(it->first.as<std::string>() == "magnetometer")
            {
                for(YAML::const_iterator magn_it = it->second.begin(); magn_it != it->second.end(); ++magn_it)
                {
                    if(magn_it->first.as<std::string>() == "mode")
                    {
                        switch(magn_it->second.as<unsigned>())
                        {
                            case 0:
                                this->magn.mode = MAGN_SHUTDOWN;
                                break;
                            case 1:
                                this->magn.mode = MAGN_SINGLE;
                                break;
                            case 2:
                                this->magn.mode = MAGN_10HZ;
                                break;
                            case 3:
                                this->magn.mode = MAGN_20HZ;
                                break;
                            case 4:
                                this->magn.mode = MAGN_50HZ;
                                break;
                            case 5:
                                this->magn.mode = MAGN_100HZ;
                                break;
                            case 6:
                                this->magn.mode = MAGN_SELF_TEST;
                                break;
                            default:
                                throw(std::runtime_error("Invalid mode chosen for magnetometer on YAML config"));
                        }
                    }
                }
            }
        }
    }
}
