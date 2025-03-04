#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-1"   // Adjust depending on your system
#define SENSOR_ADDR 0x40       // Example sensor I2C address

int main() {
    int file;
    unsigned char buffer[2];  // Adjust depending on the data length you expect
    int res;

    // Open I2C bus
    if ((file = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the bus");
        exit(1);
    }

    // Specify the I2C slave address
    if (ioctl(file, I2C_SLAVE, SENSOR_ADDR) < 0) {
        perror("Failed to connect to the sensor");
        exit(1);
    }

    // Reading data (e.g., a 2-byte value from the sensor)
    res = read(file, buffer, 2);
    if (res != 2) {
        perror("Failed to read from the I2C bus");
        exit(1);
    }

    // Assuming the sensor sends 2-byte data and you need to process it
    printf("Sensor data: %02x %02x\n", buffer[0], buffer[1]);

    close(file);
    return 0;
}
