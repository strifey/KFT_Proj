#include "client.h"

int* setup_transfer(char* ip, char* port, int _debug, int _max_packet_size){
	max_packet_size = _max_packet_size;
	recv_buff = malloc(_max_packet_size);
	send_buff = malloc(_max_packet_size);

	memset(&connInfo, 0, sizeof connInfo);
	connInfo.ai_family = AF_INET;
	connInfo.ai_socktype = SOCK_DGRAM;
	getaddrinfo(ip, port, &connInfo, &srvInfo);

	/*for loop to determine corr addr info*/
	for(pInfo = srvInfo; pInfo != NULL; pInfo = pInfo->ai_next){
		sock = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);
		if(sock <0){
			perror("Failed to create socket\n");
			continue;
		}
		printf("Created a socket to use.\n");
		return &sock;
	}
	if(pInfo == NULL){
		perror("Failed to find and bind a socket\n");
		return NULL;
	}
	return &sock;
}

int request_file(char *srv_fname, char *local_fname){
	return 0;
}
