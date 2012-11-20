#include "client.h"

extern int max_packet_size;
extern int debug;

int* setup_transfer(char* ip, char* port){
	recv_buff = malloc(sizeof(char)*max_packet_size);
	send_buff = malloc(sizeof(char)*max_packet_size);
	curr_seq = 0;

	memset(&connInfo, 0, sizeof connInfo);
	connInfo.ai_family = AF_INET;
	connInfo.ai_socktype = SOCK_DGRAM;
	connInfo.ai_protocol = IPPROTO_UDP;
	getaddrinfo(ip, port, &connInfo, &srvInfo);

	/*for loop to determine corr addr info*/
	for(pInfo = srvInfo; pInfo != NULL; pInfo = pInfo->ai_next){
		sock = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);
		if(sock <0){
			perror("Failed to create socket\n");
			continue;
		}
		if(debug)
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
	for(unsigned int i = 0; i < strlen(srv_fname); i++)
		send_buff[send_size++] = srv_fname[i];
	send_buff[send_size++] = '|';
	for(unsigned int i = 0; i< strlen(s_max_packet_size); i++)
		send_buff[send_size++] = s_max_packet_size[i];
	send_buff[send_size] = '\0';

	out_file = fopen(local_fname, "w");
	do{
		int tries = 0;
		for(; tries< MAX_TRIES ; tries++){
			if(debug)
				printf("Sending packet\n");
			sendto_dropper(sock,send_buff, send_size, 0, pInfo->ai_addr, pInfo->ai_addrlen);
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
		server_sock_len = sizeof(server_sock_in);
		recv_size = recvfrom(sock, recv_buff, max_packet_size, 0, (struct sockaddr *) &server_sock_in, &server_sock_len);
		if(!recv_size){
			perror("Server closed connection\n");
			fclose(out_file);
			return -1;
		}
		if(recv_size == -1){
			perror("recv_from failed\n");
			fclose(out_file);
			return -1;
		}

		if(debug)
			printf("Seq %d, received. %d bytes received\n", recv_buff[0], recv_size);

		//received previous packet, re-ACKING it.
		if(recv_buff[0] != curr_seq){
			if(debug)
				printf("Incorrect seq val in packet. Re-ACKing previous seq num.\n");
		}
		else{
			if(debug){
				printf("Correct seq val in packet, writing data to disk.\n");
				/*recv_buff[recv_size] = '\0';*/
				/*printf("Data in packet:\n\n %s\n\n", &recv_buff[1]);*/
			}
			//correct sequence val, reading in new data and ACKing
			//Write buffer to file
			fwrite(&recv_buff[1], sizeof(char), recv_size-1, out_file);
			memset(recv_buff, 0, max_packet_size);
			//ACKING
			memset(send_buff, 0, max_packet_size);
			send_buff[0] = curr_seq;
			send_buff[1] = 'A';
			send_size = 2;
			curr_seq ^= 1;
			if(recv_size == 1){
				if(debug)
					printf("EOF received. Listening to confirm final ACK received\n");
				break;
			}
		}
	}while(1);

	if(debug)
		printf("File finished transferring. Closing file\n");
	fclose(out_file);
	close(sock);
	return 0;
}
