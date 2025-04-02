#include <stdexcept>
#include <chrono>
#include <thread>
#include <string>
#include <cstdint>
#include <iostream>
#include <bitset>
#include <gpiod.h>


#include "include/icm20948_i2c.hpp"
#include "include/icm20948_defs.hpp"
#include "include/icm20948_utils.hpp"

#include "mraa/i2c.hpp"
#include "mraa/types.hpp"

#define G2MSQR  9.80665f
#define DEG2RAD 0.017453293f

#define MAGN_SCALE_FACTOR 0.149975574f

uint8_t int_status;
bool running = true;

gpiod_chip* chip = gpiod_chip_open_by_name("gpiochip0");
int lineNumber = 17;
gpiod_line* line = gpiod_chip_get_line(chip, lineNumber);

int LEDpin = 27;
gpiod_line* LEDLine = gpiod_chip_get_line(chip, LEDpin);

int Counter = 0;



namespace icm20948
{
    ICM20948_I2C::ICM20948_I2C(unsigned i2c_bus, unsigned i2c_address, icm20948::settings settings) : _i2c_bus(i2c_bus), 
                                                                                                  _i2c_address(i2c_address), 
                                                                                                  _current_bank(0),
                                                                                                  _i2c(i2c_bus, true),
                                                                                                  settings(settings)
    {
        _i2c.address(i2c_address);
    }


    bool ICM20948_I2C::init()
    {
        uint8_t device_id;

        bool success = true;

        success &= (_i2c.writeReg(ICM20948_REG_BANK_SEL_ADDR, ICM20948_REG_BANK_SEL_BANK0_VALUE) == mraa::SUCCESS);
        success &= _read_byte(ICM20948_WHO_AM_I_BANK, ICM20948_WHO_AM_I_ADDR, device_id);
        success &= (device_id == ICM20948_BANK0_WHO_AM_I_VALUE);
        success &= reset();
        success &= wake();
        success &= set_settings();
        _write_byte(0, ICM20948_LP_CONFIG_ADDR, 0x20);
        success &= enable_wom_interrupt(0x20);

        if (gpiod_line_request_falling_edge_events(line, "ICM20948Worker") < 0) {
            std::cerr << "Error requesting falling edge events on GPIO line " << lineNumber << std::endl;
        }

        if (gpiod_line_request_output(LEDLine, "LED", 0) < 0) {
            std::cerr << "Request LED line ouput has failed -- Womp Womp" << std::endl;
        }


    
        //success &= _disable_magnetometer();

        // Magnetometer init stage may fail once
        // Try at least 3 times before calling it off
        
        /*bool magn_initialized = false;
        for(int i = 0; i < 3; i++)
        {
            magn_initialized = _magnetometer_init();
            if(magn_initialized)
                break;
        }
        success &= magn_initialized;

        */
        return success;

       
    }


    bool ICM20948_I2C::reset()
    {
        bool success = true;

        success &= _write_bit(ICM20948_PWR_MGMT_1_BANK, ICM20948_PWR_MGMT_1_ADDR, 7, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if(success)
        {
            bool is_device_still_resetting = true;
            while(is_device_still_resetting && success)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                success &= _read_bit(ICM20948_PWR_MGMT_1_BANK, ICM20948_PWR_MGMT_1_ADDR, 7, is_device_still_resetting);
            }
        }

        // After resetting, device defaults to bank0
        if(success)
            _current_bank = 0;

        return success;
    }


    bool ICM20948_I2C::wake()
    {
        bool success = _write_bit(ICM20948_PWR_MGMT_1_BANK, ICM20948_PWR_MGMT_1_ADDR, 6, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        
        return success;
    }


    bool ICM20948_I2C::set_settings()
    {
        bool success = true;

        success &= _set_accel_sample_rate_div();
        //success &= _write_byte(ICM20948_ACCEL_CONFIG_1_BANK, ICM20948_ACCEL_CONFIG_1_ADDR,0x0F);
        success &= _set_accel_range_dlpf();
        success &= _set_gyro_sample_rate_div();
        success &= _set_gyro_range_dlpf();

        return success;
    }


    bool ICM20948_I2C::read_accel_gyro()
    {
        static uint8_t buf[12];

        if(_read_block_bytes(ICM20948_ACCEL_OUT_BANK, ICM20948_ACCEL_XOUT_H_ADDR, buf, 12))
        {
            int16_t *accel_raw, *gyro_raw;
            static uint8_t inverted_buf[12];

            for(int i = 0; i < 12; i++)
                inverted_buf[i] = buf[11-i];

            gyro_raw  = (int16_t *)(inverted_buf);
            accel_raw = (int16_t *)(inverted_buf + 6);

            accel[2] = (((float)accel_raw[0]) * _accel_scale_factor) * G2MSQR;
            accel[1] = (((float)accel_raw[1]) * _accel_scale_factor) * G2MSQR;
            accel[0] = (((float)accel_raw[2]) * _accel_scale_factor) * G2MSQR;

            gyro[2] = (((float)gyro_raw[0]) * _gyro_scale_factor) * DEG2RAD;
            gyro[1] = (((float)gyro_raw[1]) * _gyro_scale_factor) * DEG2RAD;
            gyro[0] = (((float)gyro_raw[2]) * _gyro_scale_factor) * DEG2RAD;

            return true;
        }
        else
        {
            return false;
        }
    }


    bool ICM20948_I2C::read_magn()
    {
        static uint8_t buf[6];

        if(_read_block_bytes(ICM20948_EXT_SLV_SENS_DATA_00_BANK, ICM20948_EXT_SLV_SENS_DATA_00_ADDR, buf, 6))
        {
            int16_t *mag_raw = (int16_t *)(buf);

            magn[0] = ((float)mag_raw[0]) * MAGN_SCALE_FACTOR;
            magn[1] = ((float)mag_raw[1]) * MAGN_SCALE_FACTOR;
            magn[2] = ((float)mag_raw[2]) * MAGN_SCALE_FACTOR;

            return true;
        }
        else
        {
            return false;
        }
    }


    bool ICM20948_I2C::_set_bank(uint8_t bank)
    {
        if(_current_bank != bank)
        {
            uint8_t bank_byte_value;
            switch(bank)
            {
                case 0:
                    bank_byte_value = ICM20948_REG_BANK_SEL_BANK0_VALUE;
                    break;
                case 1:
                    bank_byte_value = ICM20948_REG_BANK_SEL_BANK1_VALUE;
                    break;
                case 2:
                    bank_byte_value = ICM20948_REG_BANK_SEL_BANK2_VALUE;
                    break;
                case 3:
                    bank_byte_value = ICM20948_REG_BANK_SEL_BANK3_VALUE;
                    break;
                default:
                    throw(std::runtime_error("Invalid bank number in _set_bank(): " + std::to_string(bank)));
            }

            if(_i2c.writeReg(ICM20948_REG_BANK_SEL_ADDR, (unsigned)bank_byte_value) == mraa::SUCCESS)
            {
                _current_bank = bank;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }


    bool ICM20948_I2C::_set_accel_sample_rate_div()
    {
        uint8_t lsb, msb;
        settings.accel.sample_rate_div = 9;
        lsb =  settings.accel.sample_rate_div & 0xFF;
        msb = (settings.accel.sample_rate_div >> 8) & 0x0F;

        // std::cout << settings.accel.sample_rate_div << std::endl;
        //lsb = 0xFF;
        //msb = 0x0F;

        fprintf(stderr,"div=%x, lsb=%x, msb=%x\n",settings.accel.sample_rate_div,lsb,msb);

        bool success = true;

        success &= _write_byte(ICM20948_ACCEL_SMPLRT_DIV_1_BANK, ICM20948_ACCEL_SMPLRT_DIV_1_ADDR, msb);
        success &= _write_byte(ICM20948_ACCEL_SMPLRT_DIV_2_BANK, ICM20948_ACCEL_SMPLRT_DIV_2_ADDR, lsb);

        success &= _read_int_byte(ICM20948_ACCEL_SMPLRT_DIV_1_BANK, ICM20948_ACCEL_SMPLRT_DIV_1_ADDR,int_status);

        fprintf(stderr, "msb=%x\n",int_status);

        success &= _read_int_byte(ICM20948_ACCEL_SMPLRT_DIV_2_BANK, ICM20948_ACCEL_SMPLRT_DIV_2_ADDR,int_status);

        fprintf(stderr, "lsb=%x\n",int_status);
        return success;
    }


    bool ICM20948_I2C::_set_accel_range_dlpf()
    {
        uint8_t byte = 0;

        settings.accel.dlpf_enable = 1; //Enable ACCEL_FChoice

        byte |= !!((uint8_t)settings.accel.dlpf_enable);
        byte |= ((uint8_t)settings.accel.scale) << 1;
        byte |= ((uint8_t)settings.accel.dlpf_config) << 3;

        bool success = _write_byte(ICM20948_ACCEL_CONFIG_1_BANK, ICM20948_ACCEL_CONFIG_1_ADDR, byte);
        if(success)
            _accel_scale_factor = accel_scale_factor(settings.accel.scale);

        return success;
    }


    bool ICM20948_I2C::_set_gyro_sample_rate_div()
    {
        return _write_byte(ICM20948_GYRO_SMPLRT_DIV_BANK, ICM20948_GYRO_SMPLRT_DIV_ADDR, settings.gyro.sample_rate_div);
    }


    bool ICM20948_I2C::_set_gyro_range_dlpf()
    {
        uint8_t byte = 0;

        byte |= !!((uint8_t)settings.gyro.dlpf_enable);
        byte |= ((uint8_t)settings.gyro.scale) << 1;
        byte |= ((uint8_t)settings.gyro.dlpf_config) << 3;

        bool success = _write_byte(ICM20948_GYRO_CONFIG_1_BANK, ICM20948_GYRO_CONFIG_1_ADDR, byte);
        if(success)
            _gyro_scale_factor = gyro_scale_factor(settings.gyro.scale);

        return success;
    }


    bool ICM20948_I2C::_magnetometer_init()
    {
        bool success = true;

        success &= _magnetometer_enable();
        if(!success)
            std::cout << "Failed on _magnetometer_enable()\n";

        success &= _magnetometer_set_mode();
        if(!success)
            std::cout << "Failed on _magnetometer_set_mode()\n";

        success &= _magnetometer_configured();
        if(!success)
            std::cout << "Failed on _magnetometer_configured()\n";

        success &= _magnetometer_set_readout();
        if(!success)
            std::cout << "Failed on _magnetometer_set_readout()\n";

        return success;
    }


    bool ICM20948_I2C::_magnetometer_enable()
    {
        // Set Bypass I2C Master to false
        bool success = _write_bit(ICM20948_INT_PIN_CFG_BANK, ICM20948_INT_PIN_CFG_ADDR, 1, false);
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // No repeated start, i2c microcontroller clock = 345.60kHz
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_MST_CTRL_BANK, ICM20948_I2C_MST_CTRL_ADDR, 0x17);
            // std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        // Enable I2C Master
        if(success)
        {
            success &= _write_bit(ICM20948_USER_CTRL_BANK, ICM20948_USER_CTRL_ADDR, 5, true);
            // std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        return success;
    }


    bool ICM20948_I2C::_magnetometer_set_mode()
    {
        bool success = _write_mag_byte(AK09916_CNTL2_ADDR, (uint8_t)icm20948::MAGN_SHUTDOWN);

        if(success)
            success &= _write_mag_byte(AK09916_CNTL2_ADDR, (uint8_t)settings.magn.mode);

        return success;
    }


    bool ICM20948_I2C::_magnetometer_configured()
    {
        uint8_t mag_id;
        for(int i = 0; i < 5; i++)
        {
            bool success_read = _read_mag_byte(0x01, mag_id);
            if(success_read)
            {
                return true;
            }

            _chip_i2c_master_reset();
            std::cerr << "Magnetometer not configured properly, resetting chip I2C master\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }

        return false;
    }


    bool ICM20948_I2C::_magnetometer_set_readout()
    {
        bool success = _write_byte(ICM20948_I2C_SLV0_ADDR_BANK, ICM20948_I2C_SLV0_ADDR_ADDR, 0x8C);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV0_REG_BANK, ICM20948_I2C_SLV0_REG_ADDR, 0x11);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV0_CTRL_BANK, ICM20948_I2C_SLV0_CTRL_ADDR, 0x89);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        return success;    
    }


    bool ICM20948_I2C::_chip_i2c_master_reset()
    {
        return _write_bit(ICM20948_USER_CTRL_BANK, ICM20948_USER_CTRL_ADDR, 1, true);
    }


    bool ICM20948_I2C::_write_byte(const uint8_t bank, const uint8_t reg, const uint8_t byte)
    {
        bool success = _set_bank(bank);

        if(success)
            success &= (_i2c.writeReg(reg, byte) == mraa::SUCCESS);

        return success;
    }


    bool ICM20948_I2C::_read_byte(const uint8_t bank, const uint8_t reg, uint8_t &byte)
    {
        uint8_t ret;
        bool success = _set_bank(bank);
        
        if(success)
        {
            try
            {
                ret = _i2c.readReg(reg);
            }
            catch(const std::invalid_argument &exc)
            {
                success = false;
            }
        }

        if(success)
            byte = (uint8_t)ret;

        return success;
    }


    bool ICM20948_I2C::_write_bit(const uint8_t bank, const uint8_t reg, const uint8_t bit_pos, const bool bit)
    {
        uint8_t prexisting_byte;
        bool success = _read_byte(bank, reg, prexisting_byte);

        if(success)
        {
            // First clear bit in bit_pos: (prexisting_byte & ~((uint8_t)1 << bit_pos))
            // Then set bit to 0 or 1, based on booleanized 'bit' value: | ((!!((uint8_t)bit)) << bit_pos)
            uint8_t new_byte = (prexisting_byte & ~((uint8_t)1 << bit_pos)) | ((!!((uint8_t)bit)) << bit_pos);
            success &= _write_byte(bank, reg, new_byte);
        }
        
        return success;
    }


    bool ICM20948_I2C::_read_bit(const uint8_t bank, const uint8_t reg, const uint8_t bit_pos, bool &bit)
    {
        uint8_t prexisting_byte;
        bool success = _read_byte(bank, reg, prexisting_byte);

        if(success)
            bit = (prexisting_byte >> bit_pos) & (uint8_t)1;

        return success;
    }


    bool ICM20948_I2C::_read_block_bytes(const uint8_t bank, const uint8_t start_reg, uint8_t *bytes, const int length)
    {
        bool success = _set_bank(bank);

        if(success)
            success &= (_i2c.readBytesReg(start_reg, bytes, length) != -1);

        return success;
    }


    bool ICM20948_I2C::_write_mag_byte(const uint8_t mag_reg, const uint8_t byte)
    {
        bool success = true;

        // Set Slave4 address to magnetometer's address
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV4_ADDR_BANK, ICM20948_I2C_SLV4_ADDR_ADDR, ICM20948_MAGN_I2C_ADDR);
            // std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
            
        // Set register to write to magnetometer
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV4_REG_BANK, ICM20948_I2C_SLV4_REG_ADDR, mag_reg);
            // std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // Set byte to write to magnetometer
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV4_DO_BANK, ICM20948_I2C_SLV4_DO_ADDR, byte);
            // std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // Initiate I2C transfer between ICM20948 processor and magnetometer
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV4_CTRL_BANK, ICM20948_I2C_SLV4_CTRL_ADDR, 0x80);
            // std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // Wait for transfer to be completed (maximum wait 100ms)
        bool finished = false;
        for(int i = 0; i < 20; i++)
        {
            _read_bit(ICM20948_I2C_MST_STATUS_BANK, ICM20948_I2C_MST_STATUS_ADDR, 6, finished);
            if(finished)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        success &= finished;

        if(!finished)
            std::cerr << "Could not write to magnetometer (acknowledgement error)\n";

        return success;
    }


    bool ICM20948_I2C::_read_mag_byte(const uint8_t mag_reg, uint8_t &byte)
    {
        bool success = true;

        // Set Slave4 address to magnetometer's address
        // Also set top bit in I2C_SLV4_ADDR to 1 to signal read operation
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV4_ADDR_BANK, ICM20948_I2C_SLV4_ADDR_ADDR, ICM20948_MAGN_I2C_ADDR | 0x80);
            // std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // Set register to read from magnetometer
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV4_REG_BANK, ICM20948_I2C_SLV4_REG_ADDR, mag_reg);
            // std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // Initiate I2C transfer between ICM20948 processor and magnetometer
        if(success)
        {
            success &= _write_byte(ICM20948_I2C_SLV4_CTRL_BANK, ICM20948_I2C_SLV4_CTRL_ADDR, 0x80);
            // std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // Wait for transfer to be completed (maximum wait 100ms)
        bool finished = false;
        for(int i = 0; i < 20; i++)
        {
            _read_bit(ICM20948_I2C_MST_STATUS_BANK, ICM20948_I2C_MST_STATUS_ADDR, 6, finished);
            if(finished)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        success &= finished;

        if(!finished)
            std::cerr << "Could not read from magnetometer (acknowledgement error)\n";

        // Get byte that was read from magnetometer
        if(success)
        {
            success &= _read_byte(ICM20948_I2C_SLV4_DI_BANK, ICM20948_I2C_SLV4_DI_ADDR, byte);
        }

        return success;
    }

    //Initialise Interrupt Registers
    bool ICM20948_I2C::enable_wom_interrupt(uint8_t threshold){
        bool success = true;

        //Bank 0
        success &= _set_bank(0);

        //Enable WOM Interupt on Bit 3
        success &= _write_byte(0, ICM20948_INT_ENABLE_ADDR, 0x08);

        //Accel in low power
        success &= _write_byte(0,ICM20948_LP_CONFIG_ADDR, 0x00);

        //bank 2
        success &= _set_bank(2);
        success &= _write_byte(2, ICM20948_ACCEL_WOM_THR_ADDR, threshold);

        success &= _write_byte(2, ICM20948_ACCEL_INTEL_CTRL_ADDR, 0x03);

        success &= _write_byte(0,ICM20948_INT_PIN_CFG_ADDR, 0x90);

        return success;
    }

    bool ICM20948_I2C::check_wom_interrupt(){
        if(!_read_int_byte(0, ICM20948_INT_STATUS_ADDR, int_status)){
            std::cerr << "Couldn't read the INT_STATUS register" << std::endl;
            return false;
        }

        //std::cout << "[DEBUG] INT_STATUS Register: 0x" << std::hex << (int)int_status << std::endl;
        
        //Bit 3 refers to Wake on Motion, if int_status is the same, return true - run callback
        return (int_status & 0x08) != 0; //Bit 5 (WOM_INT) is set if motion detected

    }

    bool ICM20948_I2C::enable_DRDY_INT(){
        bool success = true;

        success &= _set_bank(0);

        success &= _write_byte(0,ICM20948_LP_CONFIG_ADDR,0x00);

        success &= _write_byte(0,ICM20948_INT_ENABLE_1_ADDR,0x01);

        success &= _write_byte(0,ICM20948_INT_PIN_CFG_ADDR, 0x10);

        return success;
    }

    bool ICM20948_I2C::check_DRDY_INT(){
        uint8_t int_status;

        if (!_read_byte(0, ICM20948_INT_STATUS_ADDR, int_status)) {
            std::cerr << "[ERROR] Failed to read INT_STATUS" << std::endl;
            return false;
        }

        std::cout << "[DEBUG] INT_STATUS: 0x" <<std::hex << (int)int_status <<std::endl;

        return (int_status & 0x20) != 0;
    }

    bool ICM20948_I2C::_read_int_byte(const uint8_t bank, const uint8_t reg, uint8_t &byte)
    {
        uint8_t ret;
        bool success = _set_bank(bank);
        
        if(success)
        {
            try
            {
                ret = _i2c.readReg(reg);
            }
            catch(const std::invalid_argument &exc)
            {
                success = false;
            }
        }

        if(success){
            byte = (uint8_t)ret;
            int_status = ret;
        }

        return success;
    }

    void ICM20948_I2C::Worker(){
        while(running) {
            // Block indefinitely until an event occurs.
            //std::cout << "Start of Loop" << std::endl;
            int ret = gpiod_line_event_wait(line, nullptr);
            if(ret < 0) {
                std::cerr << "Error waiting for event" << std::endl;
                continue;
            }
            if(ret > 0) {
                struct gpiod_line_event event;
                if(gpiod_line_event_read(line, &event) < 0) {
                    std::cerr << "Error reading event" << std::endl;
                    continue;
                }
                // Process only falling edge events (i.e. when the pin goes LOW)
                if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
                    //Check WOM clears interrupt latch, by reading INT_STATUS
                    if(check_wom_interrupt()) {
                        //replace with Callbacks
                        //std::cout << "Output" << std::endl;
                        LightLED(LEDLine);
                        //std::cout << "Hello" << std::endl;

                        Counter ++;

                        std::cout << Counter << " Hits have been detected" << std::endl;

                    }
                }
            }
        }
    }

    // void ICM20948_I2C::StartThread(){
    //     thr = std::thread(&ICM20948_I2C::Worker,this);
    // }
    
    void ICM20948_I2C::LightLED(gpiod_line* LEDLine) {
        if (gpiod_line_set_value(LEDLine, 1)< 0){
            std::cerr << "setting up LED high failed - womp womp" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (gpiod_line_set_value(LEDLine, 0) < 0) {
            std::cerr << "Canny turn it off - womp womp" << std::endl;
        }
    }

}
