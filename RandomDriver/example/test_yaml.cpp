#include <iostream>
#include "yaml-cpp/yaml.h"
#include "icm20948_utils.hpp"

int main()
{
    std::string path_to_yaml = "test.yaml";
    YAML::Node config_file_node = YAML::LoadFile(path_to_yaml.c_str());

    icm20948::settings options(config_file_node);

    std::cout << "Accel:\n" << 
                 "  sample_rate_div: " << options.accel.sample_rate_div << std::endl <<
                 "  scale: " << options.accel.scale << std::endl <<
                 "  dlpf:\n" <<
                 "    enable: " << options.accel.dlpf_enable << std::endl <<
                 "    config: " << options.accel.dlpf_config << std::endl <<
                 "Gyro:\n" <<
                 "  sample_rate_div: " << options.gyro.sample_rate_div << std::endl <<
                 "  scale: " << options.gyro.scale << std::endl <<
                 "  dlpf:\n" <<
                 "    enable: " << options.gyro.dlpf_enable << std::endl <<
                 "    config: " << options.gyro.dlpf_config << std::endl <<
                 "Magn:\n" <<
                 "  mode: " << options.magn.mode << std::endl << std::endl;

    return 0;
}