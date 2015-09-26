MAKEFLAGS+=-r

CFLAGS=-O2 -Wall -Wextra -pipe -std=c11 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

HEADERS=$(wildcard *.h)

INSTALLDIR=/usr/local/bin

.PHONY: all clean install

all: i2cread i2cwrite

i2c%: i2c%.o shared.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) i2cread i2cwrite *.o

install: i2cread i2cwrite
	install -s i2cread i2cwrite $(INSTALLDIR)
