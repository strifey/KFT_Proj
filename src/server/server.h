#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXBUFF 1000

struct addrinfo connInfo, *srvInfo, *pInfo;
struct sockaddr_storage their_addr_info;
int sock, debug;
size_t t_addr_len;
char buff[MAXBUFF];

int* bind_socket(char* port, int);
int listen_and_accept();

#endif
