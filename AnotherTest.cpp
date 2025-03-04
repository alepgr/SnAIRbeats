#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-1"    // I2C bus (usually /dev/i2c-1 on Raspberry Pi)
#define SENSOR_ADDR 0x0C        // I2C address of SEN 15335 (from i2cdetect)

int main() {
    int file;
    unsigned char buffer[6];  // Buffer to read 6 bytes (X, Y, Z accelerometer data)
    int res;

    // Open the I2C bus
    if ((file = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the bus");
        return 1;
    }

    // Specify the I2C slave address
    if (ioctl(file, I2C_SLAVE, SENSOR_ADDR) < 0) {
        perror("Failed to connect to the sensor");
        return 1;
    }

    // Initialize the sensor (if necessary, replace with actual commands)
    unsigned char init_cmd[] = {0x20, 0x27};  // Example: 0x20 could be a register to configure the sensor (e.g., setting range)
    if (write(file, init_cmd, sizeof(init_cmd)) != sizeof(init_cmd)) {
        perror("Failed to initialize the sensor");
        return 1;
    }

    // Wait for the sensor to finish initialization (if necessary)
    usleep(10000);  // Sleep for 10 milliseconds

    // Main loop to read and print data every 500 milliseconds (half a second)
    while (1) {
        // Read accelerometer data (assuming X, Y, Z data is stored at 0x28 to 0x2D)
        unsigned char reg = 0x28;  // Starting register for accelerometer data (X-axis)
        if (write(file, &reg, 1) != 1) {
            perror("Failed to write register address");
            return 1;
        }

        res = read(file, buffer, 6);  // Read 6 bytes (X, Y, Z data)
        if (res != 6) {
            perror("Failed to read from the I2C bus");
            return 1;
        }

        // Convert the raw data to accelerometer values in g (assuming ±2g range, scaling factor = 16384)
        float x_acc = (float)(buffer[0] | (buffer[1] << 8)) / 16384.0;
        float y_acc = (float)(buffer[2] | (buffer[3] << 8)) / 16384.0;
        float z_acc = (float)(buffer[4] | (buffer[5] << 8)) / 16384.0;

        // Print the accelerometer data in g
        printf("Accelerometer Data (g): X: %.2f g, Y: %.2f g, Z: %.2f g\n", x_acc, y_acc, z_acc);

        // Sleep for 500 milliseconds (half a second)
        usleep(500000);  // 500,000 microseconds = 0.5 seconds
    }

    close(file);
    return 0;
}
