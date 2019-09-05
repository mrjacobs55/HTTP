/*
 * Server.c
 *
 *  Created on: Sep 3, 2019
 *      Author: mrjacobs
 */




#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "Server.h"

#include <errno.h>

#define PORT 8080

int main(int argc, char *argv[]){
	int sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0){
		printf("Socket Creation Failed\n");
		exit(0);
	}else{
		printf("Socket opened...\n");

	}

	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(PORT);
	int addrlen = sizeof(client_addr);

	int bindStatus = bind(sock , (const struct sockaddr *)&client_addr, addrlen);

	if(bindStatus < 0){
		printf("Binding Failed\n");
		exit(0);
	}else{
		printf("Binded to port ...\n");
	}

	int listenStatus = listen(sock,5);
	if(listenStatus<0){
		printf("Listen Error\n");
		exit(0);
	}else{
		printf("Listening...\n");
	}


	int connection = accept(sock, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
	if(connection < 0){
		printf("Connection Error\n");
		exit(0);
	}else{
		printf("Connecting...\n");
	}

	char buffer[1024] = {0};
	int readStatus = read(connection, buffer, 1024);
	printf("%s\n",buffer);
	send(connection, buffer, strlen(buffer), 0);
	printf("Readback sent\n");
	return 1;
}

