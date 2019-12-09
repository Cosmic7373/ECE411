// Bliss

//#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int main() {
	int file_i2c;
	int length;
	unsigned char buffer[60] = {0};
	
	// OPEN THE I2C BUS
	char * filename = (char*)"/dev/i2c-1";

	if ((file_i2c = open(filename, O_RDWR)) < 0) {
		// ERROR
		printf("Failed to open the i2c bus");
		return 0;
	} 

	int addr = 0x04;
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// ERROR HANDLING
		return 0;
	}

	// READ BYTES 
	/*
	length = 4;
	if (read(file_i2c, buffer, length) != length) {
		printf("Fauked to read from the i2c bus.\n");
	}
	else {
		printf("Data read: %s\n", buffer);
	}*/

	// WRITE
	char buff[60] = {0};
	int x = 270;
	sprintf(buff, "%d", x);
	length = sizeof x - 1;

	for (int i = 0; i < 3; i++) {
		printf("Byte in HEX is %x\n", buff[i]);
	}
	printf("X in HEX %X DECIMAL x %d and length %d\n", x, x, length);

	if (write(file_i2c, buff, length) != length) {
		printf("Failed to write to the i2c bus.\n");
	}
	
	return 0;
}
