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

#include <errno.h>



int main(int argc, char *argv[]){
	int port = atoi(argv[argc - 1]);
	char* host = argv[argc - 2];

	char* ptr = host;
	char* file;
	int firstSlash = 0;

	while(*ptr != '\0'){
		if(*ptr == '/' && firstSlash == 0){
			firstSlash = 1;
			*ptr = '\0';
			ptr ++;
			file = ptr;
			break;
		}
		ptr++;
	}

	printf("Port: %i\n", port);
	printf("URL : %s\n", host);
	printf("File: %s\n", file);

	char* content = calloc(128, sizeof(char));
	strcpy(content, "GET /");
	strcat(content, file);
	strcat(content, " HTTP/1.1\r\nHost:");
	strcat(content, host);
	strcat(content, "\r\nAccept: text/html\r\nConnection: Close\r\n\r\n");


	//printf("%s", content);

	int sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0){
		fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
		exit(0);
	}else{
		//		printf("Socket opened...\n");
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	//	host = "localhost"; //"ccc-app-p-u01.wpi.edu";

	char* v4 = makeV4(host);
	int conversionStatus = inet_pton(AF_INET, v4, &server_addr.sin_addr);
	free(v4); 			// Free the pointer Malloc'ed for in makeV4

	if (conversionStatus < 0){
		fprintf(stderr, "Error converting to IP address: %s\n", strerror(errno));
		exit(0);
	}else {
		printf("Address Converted...\n");
	}


	int connectStatus = connect(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr) );

	if(connectStatus < 0){
		fprintf(stderr, "Error completing connection: %s\n", strerror(errno));
		exit(0);
	}else{
		printf("Connection Established ...\n");
	}


	//	if(content == NULL){
	//		content = "No Message 😢";
	//	}


	int sendStatus = send(sock , content , strlen(content)+2 , 0 );

	//printf("%s\n", content);

	//if(sendStatus == sizeof(content)){
		//printf("Success Transmitting \n");
	//}else{
		//fprintf(stderr, "%i \n%i \n", 128, sendStatus);
	//}

	//	printf("Sent:\n %s\n", content);

	FILE *output = fopen( "output.html", "w");

	free(content);
	char* buffer = calloc(128,sizeof(char));
	char* location = NULL;

	char* start = NULL;
	int write = -1;
// Write to File
	while(location == NULL){
		int readStatus = read(sock, buffer, 127);
		buffer[127] = '\0'; //Make it a string
		//printf("%s", buffer);
		location = strstr(buffer, "</html>");
		if(location == NULL){
			location = strstr(buffer, "</HTML>");
		}
		start = strstr(buffer, "<html");
		if(start == NULL){
			start = strstr(buffer, "<HTML");
		}
		if(start == NULL){
					start = strstr(buffer, "<!DOCTYPE ");
				}
		if(start != NULL){
			char* ptr = buffer;
			while(ptr < (start)){
				*ptr = ' ';
				ptr ++;
			}
			write = 0;
		}

		if(location != NULL){
			location += 7;
			while(location < (buffer + 127)){
				*location = '\0';
				location ++;
			}
		}
		if(write == 0){
			printf("%s", buffer);
			fputs(buffer,output);
		}
		free(buffer);
		buffer = calloc(127, sizeof(char));
		if(readStatus < 0){
			fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
			exit(0);
		}else{
			//		printf("Received Message ...\n");
		}



	};





	return 0;

}

struct addrinfo hints, *infoptr;

/*
 * Converts a hostname to its ipv4 address string equivalent
 */
char* makeV4(char* hostname){
	hints.ai_family = AF_INET;

	int getAddrInfoStatus = getaddrinfo(hostname, NULL, &hints, &infoptr);
	if(getAddrInfoStatus < 0){
		fprintf(stderr, "Error converting from Hostname: %s\n", strerror(errno));
		exit(0);
	}else{
		//		printf("Hostname Converted from %s to ...", hostname);
	}

	char* hostV4 = malloc(sizeof(char)*256);
	getnameinfo(infoptr->ai_addr, infoptr->ai_addrlen, hostV4, 256, NULL, 0, NI_NUMERICHOST);
	//	printf("%s" , hostV4);
	//	printf("\n");

	return hostV4;
}


