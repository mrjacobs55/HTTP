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



int main(int argc, char *argv[]){
	int port;

	if(argv[1] == NULL){
		port = 8080;
		printf("Invalid Port Number Using Default Value of %i\n", port);
	}else{
		port = atoi(argv[1]);
		if(port > 65535 || port <= 0){
			port = 8080;
			printf("Invalid Port Number Using Default Value of %i\n", port);
		}
	}

	int sock = socket(AF_INET, SOCK_STREAM,0);

	if(sock < 0){
		fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
		exit(0);
	}else{
		//printf("Socket opened...\n");

	}

	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(port);
	int addrlen = sizeof(client_addr);

	int opt = 1;
	int freeSockStatus = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	if(freeSockStatus < 0){
		fprintf(stderr, "Error freeing socket: %s\n", strerror(errno));
		exit(0);
	}

	int bindStatus = bind(sock , (const struct sockaddr *)&client_addr, addrlen);

	if(bindStatus < 0){
		fprintf(stderr, "Error binding socket: %s\n", strerror(errno));
		exit(0);
	}else{
		//printf("Binded to port ...\n");
	}

	int listenStatus = listen(sock,5);
	if(listenStatus<0){
		fprintf(stderr, "Error listening on socket: %s\n", strerror(errno));
		exit(0);
	}else{
		//printf("Listening...\n");
	}
	printf("Server is running on port %i\n", port);

	int connection = accept(sock, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
	if(connection < 0){
		fprintf(stderr, "Error listening connecting to client: %s\n", strerror(errno));
		exit(0);
	}else{
		//printf("Connecting...\n");
	}

	char buffer[1024] = {0};
	int readStatus = read(connection, buffer, 1024);

	if(readStatus < 0){
		fprintf(stderr, "Error reading from buffer: %s\n", strerror(errno));
	}
	//printf("%s\n",buffer);

	char function[8] = {0};
	char file[32] = {0};

	proccess(buffer, function, file);


	//printf("%s\n%s\n\n", function, file);


	//send(connection, buffer, strlen(buffer), 0);  //ECHO SERVER
	printf("Function: %s\n", function);
	if(!strcmp(function,"GET")){
		get(file, connection);
	}else{
		sendHeader(connection , 400);
	}

	//printf("Readback sent\n");

	int closeSocketStatus = shutdown(connection,2) | shutdown(sock,2) ;

	return closeSocketStatus;
}

int get(char* name, int connection){
	char* filename = name;
	FILE *file = fopen (filename, "r");


	if (file != NULL)
	{
		sendHeader(connection,200);                     //Send 200 OK header
		char line [1024];
		while (fgets (line, sizeof line, file) != NULL ) // Read file chunk
		{
			send(connection, line, strlen(line), 0); //Transmit file chunk
		}
		fclose (file);
	}
	else
	{
		sendHeader(connection,404);                   // Send 400 header
		fprintf(stderr, "404 sent: %s\n", strerror(errno));
	}
	return 0;
}
void sendHeader(int connection, int code){
	char* header;
	if(code == 200){
		header = "HTTP/1.1 200 OK \r\n \r\n";
	}

	if(code == 404){
		header = "HTTP/1.1 404 Not Found \r\n \r\n";
	}

	if(code == 400){
		header = "HTTP/1.1 400 Invalid Request \r\n \r\n";
	}

	send(connection, header, strlen(header), 0); //Send header
	printf("Sent Header:\n %s", header);

}
void proccess(char* request, char* function, char* file){
	char* next = request;
	char* tracker = function;
	while(*next != ' ' && tracker < (function+8)){
		*tracker = *next;
		next++;
		tracker ++;
	}
	tracker = '\0';
	next ++;
	next ++;
	tracker = file;
	while(*next != ' ' && tracker < (file+32)){
		*tracker = *next;
		next++;
		tracker ++;
	}
	tracker = '\0';


}
