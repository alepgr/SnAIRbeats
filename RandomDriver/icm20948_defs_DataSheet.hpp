#ifndef ICM20948_REGS_HPP
#define ICM20948_REGS_HPP

// Generic defines
#define ICM20948_I2C_ADDR       0x69
#define ICM20948_MAGN_I2C_ADDR  0x0C

// Banks
// Bank 0
#define ICM20948_WHO_AM_I_BANK              0

#define ICM20948_USER_CTRL_BANK             0

#define ICM20948_PWR_MGMT_1_BANK            0

#define ICM20948_INT_PIN_CFG_BANK           0

#define ICM20948_I2C_MST_STATUS_BANK        0

#define ICM20948_ACCEL_OUT_BANK             0
#define ICM20948_GYRO_OUT_BANK              0
#define ICM20948_TEMP_OUT_BANK              0
#define ICM20948_EXT_SLV_SENS_DATA_00_BANK  0
#define ICM20948_INT_ENABLE_BANK                     0

// Bank 2
#define ICM20948_GYRO_SMPLRT_DIV_BANK       2
#define ICM20948_GYRO_CONFIG_1_BANK         2
#define ICM20948_ACCEL_SMPLRT_DIV_1_BANK    2
#define ICM20948_ACCEL_SMPLRT_DIV_2_BANK    2
#define ICM20948_ACCEL_CONFIG_1_BANK        2

// Bank 3
#define ICM20948_I2C_MST_CTRL_BANK          3
#define ICM20948_I2C_SLV0_ADDR_BANK         3
#define ICM20948_I2C_SLV0_REG_BANK          3
#define ICM20948_I2C_SLV0_CTRL_BANK         3
#define ICM20948_I2C_SLV4_ADDR_BANK         3
#define ICM20948_I2C_SLV4_REG_BANK          3
#define ICM20948_I2C_SLV4_CTRL_BANK         3
#define ICM20948_I2C_SLV4_DO_BANK           3
#define ICM20948_I2C_SLV4_DI_BANK           3



// Addresses
#define ICM20948_REG_BANK_SEL_ADDR          0x7F

// Addresses: Bank 0
#define ICM20948_WHO_AM_I_ADDR              0x00

#define ICM20948_USER_CTRL_ADDR             0x03

#define ICM20948_PWR_MGMT_1_ADDR            0x06

#define ICM20948_INT_PIN_CFG_ADDR           0x0F

#define ICM20948_I2C_MST_STATUS_ADDR        0x17

#define ICM20948_ACCEL_XOUT_H_ADDR          0x2D
#define ICM20948_ACCEL_XOUT_L_ADDR          0x2E
#define ICM20948_ACCEL_YOUT_H_ADDR          0x2F
#define ICM20948_ACCEL_YOUT_L_ADDR          0x30
#define ICM20948_ACCEL_ZOUT_H_ADDR          0x31
#define ICM20948_ACCEL_ZOUT_L_ADDR          0x32

#define ICM20948_GYRO_XOUT_H_ADDR           0x33
#define ICM20948_GYRO_XOUT_L_ADDR           0x34
#define ICM20948_GYRO_YOUT_H_ADDR           0x35
#define ICM20948_GYRO_YOUT_L_ADDR           0x36
#define ICM20948_GYRO_ZOUT_H_ADDR           0x37
#define ICM20948_GYRO_ZOUT_L_ADDR           0x38

#define ICM20948_TEMP_OUT_H_ADDR            0x39
#define ICM20948_TEMP_OUT_L_ADDR            0x3A

#define ICM20948_EXT_SLV_SENS_DATA_00_ADDR  0x3B

#define ICM20948_INT_ENABLE                 0xF2
#define ICM20948_MOTION_DET_STATUS          0x62

// Addresses: Bank 2
#define ICM20948_GYRO_SMPLRT_DIV_ADDR       0x00
#define ICM20948_GYRO_CONFIG_1_ADDR         0x01
#define ICM20948_ACCEL_SMPLRT_DIV_1_ADDR    0x10
#define ICM20948_ACCEL_SMPLRT_DIV_2_ADDR    0x11
#define ICM20948_ACCEL_CONFIG_1_ADDR        0x14

// Addresses: Bank 3
#define ICM20948_I2C_MST_CTRL_ADDR          0x01
#define ICM20948_I2C_SLV0_ADDR_ADDR         0x03
#define ICM20948_I2C_SLV0_REG_ADDR          0x04
#define ICM20948_I2C_SLV0_CTRL_ADDR         0x05
#define ICM20948_I2C_SLV4_ADDR_ADDR         0x13
#define ICM20948_I2C_SLV4_REG_ADDR          0x14
#define ICM20948_I2C_SLV4_CTRL_ADDR         0x15
#define ICM20948_I2C_SLV4_DO_ADDR           0x16
#define ICM20948_I2C_SLV4_DI_ADDR           0x17



// Values
#define ICM20948_REG_BANK_SEL_BANK0_VALUE 0x00
#define ICM20948_REG_BANK_SEL_BANK1_VALUE 0x10
#define ICM20948_REG_BANK_SEL_BANK2_VALUE 0x20
#define ICM20948_REG_BANK_SEL_BANK3_VALUE 0x30

// Values: Bank 0
#define ICM20948_BANK0_WHO_AM_I_VALUE     0xEA



// Magnetometer (AK09916)
#define AK09916_CNTL2_ADDR  0x31

// Interrupt registers


#endif

