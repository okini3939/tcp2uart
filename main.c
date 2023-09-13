#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include "tcp2uart.h"

int ls;

struct {
	int port;
	char device[20];
} conf;

void *sighandler (int sig) {
	close(ls);
	exit(0);
}

int parseArg (int argc, char *argv[]) {
        int i;
        char *p;

        conf.port = 10020;
        strcpy(conf.device, "/dev/ttyS1");

        for (i = 1; i < argc; i ++) {
                p = argv[i];
                if (p[0] == '-') {
                        switch (p[1]) {
                        case 'd':
                                strncpy(conf.device, &p[2], sizeof(conf.device));
                                break;
                        case 'p':
                                conf.port = atoi(&p[2]);
                                break;
                        }
                }
        }
        return 0;
}

int main (int argc, char *argv[]) {
	int sk, ua, fd;
	fd_set fds;
	struct timeval tv;
	int n;
	char buf[1500];

	parseArg(argc, argv);
	printf("Device: %s\n", conf.device);

	signal(SIGINT, (void(*)())sighandler); // ^C
	signal(SIGKILL, (void(*)())sighandler); // kill

	ls = openTcp(conf.port_data);
	if (ls < 0) {
		return -1;
	}
loop:
	sk = acceptTcp(ls);
	if (sk < 0) {
		return -1;
	}

	ua = openUart(conf.device);
	if (ua < 0) {
		close(sk);
		close(ls);
		return -1;
	}
	printf("ls=%d sk=%d ua=%d\n", ls, sk, ua);

	for (;;) {
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(sk, &fds);
		FD_SET(ua, &fds);
		fd = sk > ua ? sk : ua;
		n = select(fd + 1, &fds, NULL, NULL, &tv);

		if (n == 0) {
			// timeout
			continue;
		} else
		if (n < 0) {
			// signal
			if (errno == EINTR) continue;
			break;
		}

		if (FD_ISSET(sk, &fds)) {
			n = recvTcp(sk, buf, sizeof(buf));
			if (n <= 0) {
				break;
			}
			sendUart(ua, buf, n);
			buf[n] = 0;
			printf(">> %s\n", buf);
		}
		if (FD_ISSET(ua, &fds)) {
			n = recvUart(ua, buf, sizeof(buf));
			if (n <= 0) {
				break;
			}
			sendTcp(sk, buf, n);
			buf[n] = 0;
			printf("<< %s\n", buf);
		}
	}

	close(ua);
	close(sk);
	goto loop;

	close(ls);
	return 0;
}

