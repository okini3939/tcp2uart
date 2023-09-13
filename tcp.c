#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "tcp2uart.h"


int openTcp (int port) {
	int ls;
        struct sockaddr_in my_addr;
        fd_set fds, readfds;
        struct timeval tv;

	if (port < 100) return -1;

        ls = socket(AF_INET, SOCK_STREAM, 0);
        if (ls == -1) {
                fprintf(stderr, "Error socket %d\n", errno);
                return -1;
        }

        my_addr.sin_family = AF_INET;
        my_addr.sin_port   = htons(port);
        my_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(ls, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
                fprintf(stderr, "Error bind %d\n", errno);
                return -1;
        }

        if (listen(ls, 1) == -1) {
                fprintf(stderr, "Error listen %d\n", errno);
                close(ls);
                return -1;
        }
	return ls;
}

int acceptTcp (int ls) {
        int sk, paddrlen;
        struct sockaddr_in peer;
        int n;

	if (ls < 0) return -1;

        paddrlen = sizeof(peer);
        sk = accept(ls, (struct sockaddr*)&peer, &paddrlen);
        if (sk == -1) {
                fprintf(stderr, "Error accept %d\n", errno);
                close(ls);
                return -1;
        }
        n = ntohl(peer.sin_addr.s_addr);
//        printf("accept %d %d.%d.%d.%d:%d\n", sk, (n >> 24) & 0xff, (n >> 16) & 0xff, (n >> 8) & 0xff, n & 0xff, peer.sin_port);
	return sk;
}

void closeTcp (int sk) {
	if (sk < 0) return;
	close(sk);
}

int recvTcp (int sk, char *buf, int len) {
        int n;

	if (sk < 0) return -1;
        n = recv(sk, buf, len, 0);
        return n;
}

int sendTcp (int sk, char *buf, int len) {
	int n;

	if (sk < 0) return -1;
        n = send(sk, buf, len, 0);
        return n;
}

