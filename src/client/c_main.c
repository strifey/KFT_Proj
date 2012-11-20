#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "client.h"

void parse_parameters(int argc, char*argv[]);
char *srv_fname, *local_fname, *srv_ip, *srv_port = NULL;
int percent_loss = 0;

int main(int argc, char*argv[]){
	parse_parameters(argc, argv);
	set_dropper(percent_loss);
	setup_transfer(srv_ip, srv_port);
	int result = request_file(srv_fname, local_fname);
	if(debug){
		if(!result)
			printf("File successfully received!\n");
		else if(result == -1)
			printf("There was a connection issue during file transfer\n");
		else if(result == -2)
			printf("Client timed out too many times waiting on server to respond.\n");
	}
	return result;
}

void parse_parameters(int argc, char*argv[]){
	if(argc < 7 || argc > 8){
		perror("Incorrect amount of command arguments\n");
		exit(1);
	}
	srv_ip = argv[1];
	srv_port = argv[2];
	srv_fname = argv[3];
	local_fname = argv[4];
	max_packet_size = atoi(argv[5]);
	s_max_packet_size = argv[5];
	percent_loss = atoi(argv[6]);
	if(!max_packet_size){
		perror("Please input a valid number for max packet size\n");
		exit(1);
	}
	if(!percent_loss && strcmp(argv[6], "0")){
		perror("Please input a valid number for percent loss size\n");
		exit(1);
	}
	if(argc == 8){
		if(strcmp(argv[7], "-d")){
			perror("The final flag may only be -d. Please either pass no flag or pass the correct one.\n");
			exit(1);
		}
		debug = 1;
	}
}
