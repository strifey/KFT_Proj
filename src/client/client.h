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

struct addrinfo connInfo, *srvInfo, *pInfo;
int sock;

int* bind_socket(char* port, int debug);

#endif
