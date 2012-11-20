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

#define INITBUFF 1000
#define MAX_TRIES 5
#define TIMEOUT_S 0
#define TIMEOUT_U 100000

struct addrinfo connInfo, *srvInfo, *pInfo;
struct sockaddr_in client_addr_info;
int sock, numbytes, fsize, debug;
int c_addr_len, recv_size, final_ack, send_EOF;
char *recv_buff;
char *send_buff;
unsigned int send_size, max_packet_size;
char *curr_filename;
char curr_seq;
FILE *req_file;

int *bind_socket(char *port);
int listen_and_accept();
int send_file();

#endif
