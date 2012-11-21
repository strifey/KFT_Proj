#include "server.h"

extern int debug;

int* bind_socket(char* port){
	//set up socket info
	memset(&connInfo, 0, sizeof connInfo);
	connInfo.ai_family = AF_INET;
	connInfo.ai_socktype = SOCK_DGRAM;
	connInfo.ai_flags = AI_PASSIVE;
	connInfo.ai_protocol = IPPROTO_UDP;
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
	//set recv_buff to some initial size
	recv_buff = malloc(sizeof(char)*INITBUFF);
	//initial wait for new connection
	c_addr_len = sizeof(client_addr_info);
	numbytes= recvfrom(sock, recv_buff, INITBUFF-1, 0, (struct sockaddr *)&client_addr_info, &c_addr_len);
	//parse data about send
	//like filename and max_packet_size
	curr_filename = strtok(recv_buff, "|");
	max_packet_size = atoi(strtok(NULL, "|"));
	recv_buff = malloc(sizeof(char)*max_packet_size);
	send_buff = malloc(sizeof(char)*max_packet_size);
	if(debug){
		printf("Filename requested:  %s\n", curr_filename);
		printf("Max packet size:  %i, %i\n", max_packet_size, max_packet_size);
	}
	if(curr_filename == NULL || !max_packet_size){
		perror("Error parsing data from client.\n");
		return -1;
	}
	//Open file and find out size of file about to be sent
	req_file = fopen(curr_filename, "r");
	if(req_file == NULL){
		perror("Error opening file. Aborting\n");
		return -1;
	}
	int result = send_file();
	free(curr_filename);
	free(recv_buff);
	free(send_buff);
	fclose(req_file);
	return result;
}

int send_file(){
	final_ack = curr_seq = send_EOF= 0;

	memset(send_buff, 0, max_packet_size);
	send_buff[0] = curr_seq;
	send_size = fread(&send_buff[1], sizeof(char), max_packet_size -1, req_file);
	send_size++;
	if(send_size < max_packet_size)
		send_EOF = 1;
	/*if(debug){
		send_buff[send_size+1] = '\0';
		printf("Data read from file: \n%s\n\n", &send_buff[1]);
	}*/

	for(;;){
		int tries = 0;
		for(; tries< MAX_TRIES ; tries++){
			sendto_dropper(sock,send_buff, send_size, 0, (struct sockaddr *)&client_addr_info, c_addr_len);
			struct timeval tv;
			tv.tv_sec = TIMEOUT_S;
			tv.tv_usec = TIMEOUT_U;

			fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(sock, &readfds);
			int sel_ret = select(sock+1, &readfds, NULL, NULL, &tv);
			if(sel_ret>0){
				break;
			}
			if(debug)
				printf("Nothing to read. Resending packet. Try %i\n", tries);
		}
		//timed out and couldn't make connection
		if(tries == MAX_TRIES)
			return -2;
		c_addr_len = sizeof(client_addr_info);
		recv_size = recvfrom(sock, recv_buff, max_packet_size, 0, (struct sockaddr *) &client_addr_info, &c_addr_len);
		if(!recv_size){
			perror("Client closed connection\n");
			return -1;
		}
		if(recv_size == -1){
			perror("recv_from failed\n");
			return -1;
		}
		//ACK for previous packet received. sending next one
		if(recv_size == 2 && recv_buff[0] == curr_seq && recv_buff[1] == 'A'){
			if(debug)
				printf("Seq %d, received. Looking for %d. Sending next packet\n", recv_buff[0], curr_seq);
			if(send_EOF){
				//Final ACK, stopping
				break;
			}
			else{
				//Reading and prepping buffer for sending.
				memset(send_buff, 0, max_packet_size);
				curr_seq ^= 1;
				send_buff[0] = curr_seq;
				send_size = fread(&send_buff[1], sizeof(char), max_packet_size -1, req_file);
				send_size++; //make allocation for seq_num
				/*if(debug){
					send_buff[send_size+1] = '\0';
					printf("Data read from file: \n%s\n\n", &send_buff[1]);
				}*/
				if(send_size < max_packet_size-1)
					send_EOF = 1;
			}
		}
		if(debug)
			printf("Seq %d, received. Looking for %d. Resending previous packet\n", recv_buff[0], curr_seq);
		//Either out of sequence or some other anomaly, resending old packet.
	}
	if(debug)
		printf("Sending EOF to tell Client file is done.\n");
	memset(send_buff, 0, max_packet_size);
	curr_seq ^= 1;
	send_buff[0] = curr_seq;
	send_size = 1;
	sendto_dropper(sock,send_buff, send_size, 0, (struct sockaddr *)&client_addr_info, c_addr_len);
	int tries = 0;
	for(; tries< MAX_TRIES ; tries++){
		struct timeval tv;
		tv.tv_sec = TIMEOUT_S;
		tv.tv_usec = TIMEOUT_U;

		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		int sel_ret = select(sock+1, &readfds, NULL, NULL, &tv);
		if(sel_ret>0){
			recvfrom(sock, recv_buff, max_packet_size, 0, (struct sockaddr *) &client_addr_info, &c_addr_len);
			sendto_dropper(sock,send_buff, send_size, 0, (struct sockaddr *)&client_addr_info, c_addr_len);
			if(debug)
				printf("Must've lost final EOF packet. Resending it.\n");
			//tries = 0;
		}
		else if(debug)
			printf("Nothing to read. Continue waiting, just in case.\n");
	}
}
