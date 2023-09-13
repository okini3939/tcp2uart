#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <errno.h>
#include "tcp2uart.h"


int openUart (char *dev_path) {
	int fd;
	fd = open(dev_path, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		fprintf(stderr, "Error open %d\n", errno);
	}
	return fd;
}

void closeUart (int fd) {
	if(fd < 0) return;
	close(fd);
}

void setBaud (int fd, uint32_t baud, int flow) {
	if(fd < 0) return;
	int tio_buad;
	
	switch(baud) {
	case 9600:
		tio_buad = B9600;
		break;
	case 19200:
		tio_buad = B19200;
		break;
	case 38400:
		tio_buad = B38400;
		break;
	case 57600:
		tio_buad = B57600;
		break;
	case 115200:
		tio_buad = B115200;
		break;
	default:
		/* Above values should be enough. */
		tio_buad = B9600;
		break;
	}

	struct termios tio = {
		.c_cflag = tio_buad | CS8 | CLOCAL | CREAD | (flow ? CRTSCTS : 0),
		.c_iflag = 0,
		.c_oflag = 0,
		.c_lflag = NOFLSH,
		.c_cc = {0},
	};

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &tio);
}

int sendUart (int fd, char *buf, int len) {
	int n;
	if (fd < 0) return -1;
	n = write(fd, buf, len);
	return n;
}

int recvUart (int fd, char *buf, int len) {
	int n;
	if (fd < 0) return -1;
	n = read(fd, buf, len);
	return n;
}

