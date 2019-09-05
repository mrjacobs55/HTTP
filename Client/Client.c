/*
 * Client.c
 *
 *  Created on: Sep 3, 2019
 *      Author: mrjacobs
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "Client.h"

#define PORT 8080

int main(int argc, char *argv[]){

	char* message = argv[1];

	int sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0){
		printf("Socket Creation Failed\n");
		exit(0);
	}else{
		printf("Socket opened...\n");
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);


	int conversionStatus = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	if (conversionStatus < 0){
		printf("\nInvalid address");
		exit(0);
	}else {
		printf("Address Converted...\n");
	}


	int connectStatus = connect(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr) );

	if(connectStatus < 0){
		printf("Connection Failed\n");
		exit(0);
	}else{
		printf("Connection Established ...\n");
	}
	char buffer[1024] = {0};

	if(message == NULL){
		message = "No Message 😢";
	}
	send(sock , message , strlen(message) , 0 );
	printf("Sent\n");


	int readStatus = read(sock, buffer, 1024);
	printf("%s\n", buffer);


	return 0;

}




