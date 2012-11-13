#include "server.h"

int* bind_socket(char* port, int _debug){
	debug = _debug;

	memset(&connInfo, 0, sizeof connInfo);
	connInfo.ai_family = AF_INET;
	connInfo.ai_socktype = SOCK_STREAM;
	connInfo.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, port, &connInfo, &srvInfo);

	/*for loop to determine corr addr info*/
	for(pInfo = srvInfo; pInfo != NULL; pInfo = pInfo->ai_next){
		sock = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);
		if(sock <0){
			continue;
		}
		if(bind(sock, pInfo->ai_addr, pInfo->ai_addrlen) < 0){
			close(sock);
			continue;
		}
		return &sock;
	}
	if(pInfo == NULL){
		return NULL;
	}
	return &sock;
}

int listen_and_accept(){
	t_addr_len = sizeof(their_addr_info);
	int retval = recvfrom(sock, buff, MAXBUFF-1, 0, (struct sockaddr *)&their_addr_info, &t_addr_len);
	return retval;
}
