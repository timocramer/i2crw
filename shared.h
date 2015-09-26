#ifndef I2CRW_SHARED
#define I2CRW_SHARED

#include <stdint.h>

enum show_method {
	RAW,
	NUMBERS
};

extern char *program_name;
extern int i2c_fd;

void open_connection(const char *arg);
void set_slave_id(const char *arg);

void error_message(const char *message);
void error_message_errno(const char *message);

#endif
