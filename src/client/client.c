#include "client.h"

int* bind_socket(char* port, int debug){
	memset(&connInfo, 0, sizeof connInfo);
	connInfo.ai_family = AF_INET;
	connInfo.ai_socktype = SOCK_STREAM;
	connInfo.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, port, &connInfo, &srvInfo);

	/*for loop to determine corr addr info*/
	for(pInfo = srvInfo; pInfo != NULL; pInfo = pInfo->ai_next){
		sock = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);
		if(sock <0){
			perror("Failed to create socket\n");
			continue;
		}
		if(bind(sock, pInfo->ai_addr, pInfo->ai_addrlen) < 0){
			close(sock);
			perror("Failed to bind socket\n");
			continue;
		}
		printf("Found a socket and bound\n");
		return &sock;
	}
	if(pInfo == NULL){
		perror("Failed to find and bind a socket\n");
		return NULL;
	}
	return &sock;
}
