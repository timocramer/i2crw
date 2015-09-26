#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <alloca.h>

#include "shared.h"

static void show_usage(void) {
	printf(
		"Usage: %s -n i2cbus chip-address [number...]\n"
		"       %s -r i2cbus chip-address [string...]\n"
		"       %s -h\n"
		"\n"
		"\ti2cbus: a number (23 opens /dev/i2c-23) or a path\n"
		"\tchip-address: an integer between 0x03 and 0x77\n"
		"\n"
		"\t-n: numerical input\n"
		"\t-r: raw input\n"
		"\t-h: show this help\n",
		program_name,
		program_name,
		program_name
	);
}

int main(int argc, char *argv[]) {
	int c;
	ssize_t status;
	uint8_t *data;
	size_t data_length;
	enum show_method input_method = NUMBERS;
	
	program_name = argv[0];
	
	while(1) {
		c = getopt(argc, argv, "nrh");
		if(c == -1)
			break;
		
		switch(c) {
			case 'n':
				input_method = NUMBERS;
				break;
			case 'r':
				input_method = RAW;
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
	
	/* what do we want to write? when no value is given, error */
	if(optind >= argc) {
		error_message("you have to provide at least one parameter to send");
	}
	
	data = NULL;
	data_length = 0;
	for(; optind < argc; ++optind) {
		switch(input_method) {
		case NUMBERS:
			++data_length;
			data = realloc(data, data_length);
			if(data == NULL) {
				error_message("memory allocation failed");
			}
			data[data_length - 1] = strtol(argv[optind], NULL, 0);
			
			break;
		case RAW:
			data_length += strlen(argv[optind]);
			data = realloc(data, data_length);
			if(data == NULL) {
				error_message("memory allocation failed");
			}
			strcat(data, argv[optind]);
			
			break;
		}
	}
	
	status = write(i2c_fd, data, data_length);
	if(status != data_length) {
		error_message_errno("write error");
	}
	
	return 0;
}
