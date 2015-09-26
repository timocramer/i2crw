#include "shared.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

char *program_name;
int i2c_fd;

void error_message_errno(const char *message) {
	fprintf(stderr, "%s: %s: %s\n", program_name, message, strerror(errno));
	exit(1);
}

void error_message(const char *message) {
	fprintf(stderr, "%s: %s\n", program_name, message);
	exit(1);
}

#define ALLOWED_DIGITS 2
void open_connection(const char *arg) {
	const char *path;
	long i2c_number;
	char i2c_number_path[sizeof("/dev/i2c-") + ALLOWED_DIGITS];
	
	i2c_number = strtol(arg, NULL, 0);
	if(i2c_number > 0 || strcmp(arg, "0") == 0) {
		snprintf(i2c_number_path, sizeof(i2c_number_path), "/dev/i2c-%ld", i2c_number);
		
		path = i2c_number_path;
	}
	else {
		path = arg;
	}
	
	i2c_fd = open(path, O_RDWR);
	if(i2c_fd == -1) {
		error_message_errno("error opening i2c-connection");
	}
}

void set_slave_id(const char *arg) {
	int status;
	uint8_t slave_id;
	
	slave_id = strtol(arg, NULL, 0);
	if(slave_id < 0x03 || slave_id > 0x77) {
		fprintf(stderr, "slave id has to be between 0x03 and 0x77\n");
		exit(1);
	}
	
	status = ioctl(i2c_fd, I2C_SLAVE, slave_id);
	if(status == -1) {
		error_message_errno("error setting slave id");
	}
}
