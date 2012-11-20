#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void parse_parameters(int argc, char*argv[]);

int percent_loss;
char *port;
int *binded_sock;

int main(int argc, char*argv[]){
	parse_parameters(argc, argv);
	set_dropper(percent_loss);
	binded_sock = bind_socket(port);
	if(binded_sock == NULL){
		perror("Could not connect to a socket. Closing\n");
		exit(1);
	}
	printf("Found a socket and bound\n");
	while(1){
		int send_result = listen_and_accept();
		if(!send_result)
			printf("File sent successfully\n");
		if(send_result == -1)
			printf("Connection issues during file transfer\n");
		if(send_result == -2)
			printf("Client timed-out during file transfer\n");
	}
}

void parse_parameters(int argc, char*argv[]){
	if(argc < 3 || argc > 4){
		perror("Incorrect number of variables\n");
		exit(1);
	}
	if(argc == 4){
		if(strcmp(argv[3], "-d")){
		perror("Invalid final parameter. Must be -d flag for debug mode.\n");
		exit(1);
		}
		debug = 1;
	}
	percent_loss = atoi(argv[2]);
	port = argv[1];
	if(!percent_loss && strcmp(argv[2], "0")){
		perror("Invalid percent_loss number. Please enter a valid number for the percent_loss\n");
		exit(1);
	}
}
