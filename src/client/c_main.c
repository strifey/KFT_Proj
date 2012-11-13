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
int max_packet_size, percent_loss, debug = 0;

int main(int argc, char*argv[]){
	parse_parameters(argc, argv);
	set_dropper(percent_loss);
	setup_transfer(srv_ip, srv_port, debug, max_packet_size);
	int result = request_file(srv_fname, local_fname);

	return 0;
}

void parse_parameters(int argc, char*argv[]){
	if(argc < 6 || argc > 7){
		perror("Incorrect amount of command arguments\n");
		exit(1);
	}
	srv_ip = argv[1];
	srv_port = argv[2];
	srv_fname = argv[3];
	local_fname = argv[4];
	max_packet_size = atoi(argv[5]);
	if(!max_packet_size){
		perror("Please input a valid number for max packet size\n");
		exit(1);
	}
	if(argc == 7){
		if(strcmp(argv[6], "-d")){
			perror("The final flag may only be -d. Please either pass no flag or pass the correct one.\n");
			exit(1);
		}
		debug = 1;
	}
}
