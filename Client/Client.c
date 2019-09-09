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
#include <netdb.h>
#include <string.h>
#include "Client.h"

#define PORT 3534

int main(int argc, char *argv[]){

	char* host = argv[1];
	char* content = "GET /something HTTP/1.1\r\n Host: localhost\r\n \r\n";
	printf("%s", content);

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

	host = "ccc-app-p-u01.wpi.edu";

	int conversionStatus = inet_pton(AF_INET, makeV4(host), &server_addr.sin_addr);
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
	char buffer[16384] = {0};

	if(content == NULL){
		content = "No Message 😢";
	}
	send(sock , content , strlen(content) , 0 );
	printf("Sent\n");


	int readStatus = read(sock, buffer, 16384);
	if(readStatus < 0){
		printf("Read Failed");
		exit(0);
	}else{
		printf("Received Message ...\n");
	}
	printf("%s\n", buffer);



	return shutdown(sock,2);

}

struct addrinfo hints, *infoptr;

/*
 * Converts a hostname to its ipv4 address string equivalent
 */
char* makeV4(char* hostname){
	hints.ai_family = AF_INET;

	int getAddrInfoStatus = getaddrinfo(hostname, NULL, &hints, &infoptr);
	if(getAddrInfoStatus < 0){
		printf("Hostname Conversion Failed\n ");
		exit(0);
	}else{
		printf("Hostname Converted from %s to ...", hostname);
	}

	char* hostV4 = malloc(sizeof(char)*256);
	getnameinfo(infoptr->ai_addr, infoptr->ai_addrlen, hostV4, 256, NULL, 0, NI_NUMERICHOST);
	printf("%s" , hostV4);
	printf("\n");

	return hostV4;
}


