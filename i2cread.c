#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <alloca.h>

#include "shared.h"

static void show_usage(void) {
	printf(
		"Usage: %s [-n|-r] i2cbus chip-address [length]\n"
		"       %s -h\n"
		"\n"
		"\ti2cbus: a number (23 opens /dev/i2c-23) or a path\n"
		"\tchip-address: an integer between 0x03 and 0x77\n"
		"\n"
		"\t-n: numerical output\n"
		"\t-r: raw output\n"
		"\t-h: show this help\n",
		program_name,
		program_name
	);
}

static void output_hex(const uint8_t data[], size_t length) {
	size_t i;
	
	for(i = 0; i < length; ++i) {
		printf("0x%02x\n", data[i]);
	}
}

static void output_raw(const uint8_t data[], size_t length) {
	size_t i;
	
	for(i = 0; i < length; ++i) {
		putchar(data[i]);
	}
}

int main(int argc, char *argv[]) {
	int c;
	long read_length;
	ssize_t status;
	uint8_t *data;
	enum show_method output_method = NUMBERS;
	
	program_name = argv[0];
	
	while(1) {
		c = getopt(argc, argv, "nrh");
		if(c == -1)
			break;
		
		switch(c) {
		case 'n':
			output_method = NUMBERS;
			break;
		case 'r':
			output_method = RAW;
			break;
		case 'h':
			show_usage();
			return 1;
		}
	}
	
	/* open i2c-connection */
	if(optind >= argc) {
		show_usage();
		return 1;
	}
	open_connection(argv[optind]);
	++optind;
	
	/* set i2c-slave_id */
	if(optind >= argc) {
		error_message("missing chip-address");
	}
	set_slave_id(argv[optind]);
	++optind;
	
	/* how much do we want to read? when no value is given, assume one byte */
	if(optind >= argc) {
		read_length = 1;
	}
	else {
		read_length = strtol(argv[optind], NULL, 0);
		if(read_length < 1) {
			error_message("you want to read at least one byte");
		}
	}
	
	/* allocate space to read and read */
	data = alloca(read_length);
	status = read(i2c_fd, data, read_length);
	if(status != read_length) {
		error_message_errno("read error");
	}
	
	/* output data */
	switch(output_method) {
	case NUMBERS:
		output_hex(data, read_length);
		break;
	case RAW:
		output_raw(data, read_length);
		break;
	}
	
	return 0;
}
