#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../dropper.h"

#define MAXBUFF 1000
#define TIMEOUT_S 2
#define TIMEOUT_U 500000
#define ACK "ACK"
#define NACK "NACK"

struct addrinfo connInfo, *srvInfo, *pInfo;
struct sockaddr_storage client_addr_info;
int sock, curr_seq, numbytes;
socklen_t c_addr_len;
char recv_buff[MAXBUFF];
char send_buff[MAXBUFF];
unsigned int send_size;

int* bind_socket(char* port, int);
int listen_and_accept();
void get_and_ack();

#endif
