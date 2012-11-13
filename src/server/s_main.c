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

int percent_loss, debug = 0;
char *port;
int * binded_sock;

int main(int argc, char*argv[]){
	parse_parameters(argc, argv);
	set_dropper(percent_loss);
	binded_sock = bind_socket(port, debug);
	if(binded_sock == NULL){
		perror("Could not connect to a socket. Closing\n");
		exit(1);
	}
	printf("Found a socket and bound\n");
	while(1){
		//reset
		if(listen_and_accept() == -1){
			perror("Error occurred while awaiting the initial receive. Closing.\n");
			exit(1);
		}
		do{
			//send next bit of file
		}while(numbytes > 0);

		if(!numbytes){
			perror("Client closed connection\n");
			exit(1);
		}else if(numbytes == -1){
			perror("An error occurred during file transfer\n");
			exit(1);
		}else if(numbytes == -2){
			perror("Client connection timed out. Closing connection.\n");
			exit(1);
		}
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
	if(!percent_loss){
		perror("Invalid percent_loss number. Please enter a valid number for the percent_loss\n");
		exit(1);
	}
}
