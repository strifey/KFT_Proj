#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../dropper.h"

struct addrinfo connInfo, *srvInfo, *pInfo;
char *recv_buff, *send_buff;
int sock, send_size, max_packet_size;


int* setup_transfer(char* ip, char* port, int _debug, int _max_packet_size);
int request_file(char *srv_fname, char *local_fname);

#endif
