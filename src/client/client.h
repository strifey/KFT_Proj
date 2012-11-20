#ifndef CLIENT_H
#define CLIENT_H

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

#define MIN_PACKET_SIZE 25
#define MAX_TRIES 5
#define TIMEOUT_S 0
#define TIMEOUT_U 100000

struct addrinfo connInfo, *srvInfo, *pInfo;
struct sockaddr_in server_sock_in;
int server_sock_len;
char *recv_buff, *send_buff, *s_max_packet_size;
int sock, send_size, recv_size, max_packet_size, file_done, debug;
char curr_seq;
FILE *out_file;


int* setup_transfer(char* ip, char* port);
int request_file(char *srv_fname, char *local_fname);

#endif
