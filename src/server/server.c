#include "server.h"

int* bind_socket(char* port, int _debug){
	
	memset(&connInfo, 0, sizeof connInfo);
	connInfo.ai_family = AF_INET;
	connInfo.ai_socktype = SOCK_DGRAM;
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
	c_addr_len = sizeof(client_addr_info);
	numbytes= recvfrom(sock, recv_buff, MAXBUFF-1, 0, (struct sockaddr *)&client_addr_info, &c_addr_len);
	strcpy(send_buff, ACK);
	send_size = 4;
	sendto_dropper(sock, send_buff, send_size, 0, (struct sockaddr *)&client_addr_info, c_addr_len);
	return numbytes;
}

void get_and_ack(){
	struct timeval tv;
	tv.tv_sec = TIMEOUT_S;
	tv.tv_usec = TIMEOUT_U;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(sock+1, &readfds);

	select(sock+1, &readfds, NULL, NULL, &tv);

}
