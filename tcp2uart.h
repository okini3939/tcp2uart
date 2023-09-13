#ifndef __TCP2UART_H__
#define __TCP2UART_H__

int openTcp (int port);
int acceptTcp (int ls);
void closeTcp (int sk);
int recvTcp (int sk, char *buf, int len);
int sendTcp (int sk, char *buf, int len);

int openUart (char *dev_path);
void closeUart (int fd);
void setBaud (int fd, uint32_t baud, int flow);
int sendUart (int fd, char *buf, int len);
int recvUart (int fd, char *buf, int len);

#endif

