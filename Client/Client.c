/*
 * Client.c
 *
 *  Created on: Sep 3, 2019
 *      Author: mrjacobs
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "Client.h"

#include <errno.h>


/*
 * ./http_client [-options] server_url port_number
 * -p prints the RTT for accessing the URL on the terminal

 *Output  Displays Servers Response on Terminal
 *Output -p Prints RTT followed by Normal output

 */
int main(int argc, char *argv[]){

	int port;

	if(argv[argc - 1] == NULL){
			port = 8080;
			printf("Invalid Port Number Using Default Value of %i\n", port);
		}else{
			port = atoi(argv[1]);
			if(port > 65535 || port <= 0){
				port = 8080;
				printf("Invalid Port Number Using Default Value of %i\n", port);
			}
		}

	char* host = argv[argc - 2];
	int RTT = 0;
	if(argc > 3){
		if(strstr(argv[1] , "-p")){
			RTT = 1;  				//Print RTT
		}
	}

	char* ptr = host;
	char* file;
	int firstSlash = 0;


	while(*ptr != '\0'){ 							//Not at end of string
		if(*ptr == '/' && firstSlash == 0){			//If the url is completed
			firstSlash = 1;							//We hit the start of the file location
			*ptr = '\0';							//Seperate into 2 strings
			ptr ++;									//Move to the first character of the file
			file = ptr;								//Save it
			break;									//Get out of the loop
		}
		ptr++;
	}

	//	printf("Port: %i\n", port);
	//	printf("URL : %s\n", host);
	//	printf("File: %s\n", file);


	char* requestHeader = calloc(128, sizeof(char));

	if(createHeader(file, host, requestHeader) == NULL){	//Create Header
		fprintf(stderr, "Header Corrupted");
		exit(-1);
	}


	//printf("%s", content);

	int sock = socket(AF_INET, SOCK_STREAM,0);				//Open Socket
	if(sock < 0){
		fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
		exit(-1);
	}else{
		//		printf("Socket opened...\n");
	}


	struct sockaddr_in server_addr;						//Server Address
	server_addr.sin_family = AF_INET;						//Internet Address
	server_addr.sin_port = htons(port);						//At PORT

	//	host = "localhost";							//OVERRIDE Host for testing


	char* v4 = makeV4(host);					//Convert to String of IPV4 address

	int conversionStatus = inet_pton(AF_INET, v4, &server_addr.sin_addr); //Place the string ipv4 address into server_addr address field

	free(v4); 			// Free the pointer Malloc'ed for in makeV4

	if (conversionStatus < 0){
		fprintf(stderr, "Error converting to IP address: %s\n", strerror(errno));
		exit(-1);
	}else {
		//printf("Address Converted...\n");
	}

	struct timeval stop, start;
	gettimeofday(&start, NULL);  	//Start Time

	int connectStatus = connect(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr) ); //Connect to Server

	gettimeofday(&stop, NULL);
	if(RTT){
		printf("\nRTT is %'.3f MilliSeconds\n\n", (double) (stop.tv_sec - start.tv_sec) * 1000 + (double) (stop.tv_usec - start.tv_usec) / 1000);
	}
	if(connectStatus < 0){
		fprintf(stderr, "Error completing connection: %s\n", strerror(errno));
		exit(-1);
	}else{
		//printf("Connection Established ...\n");
	}


	//	if(content == NULL){
	//		content = "No Message 😢";
	//	}


	int sendStatus = send(sock , requestHeader, strlen(requestHeader) , 0 );  //Send request header to server

	//printf("%s\n", content);

	if(sendStatus != strlen(requestHeader)){
		fprintf(stderr, "Error Transmitting: %s\n", strerror(errno));
		exit(-1);
	}

	//	printf("Sent:\n %s\n", content);

	FILE *output = fopen( "output.html", "w");				//Set output file

	free(requestHeader);  					//It was transmitted and no longer needed

	readSocket(sock , output);		//Read the servers response

};


struct addrinfo hints, *infoptr;

/*
 * Converts inHostname to its ipv4 address string equivalent
 * If inHostname is NULL localhost is used
 */
char* makeV4(char* inHostname){
	char* hostname = "localhost";
	if(inHostname != NULL){
		hostname = inHostname;
	}
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

/**
 * Creates Header to request inFile at inHost and places it at the location of header.
 * If inFIle or inHost are NULL they default to index.html at localhost respectively
 * If header is NULL the function returns NULL
 */
char* createHeader(char* inFile, char* inHost, char* header){
	char* file = "index.html";
	char* host = "localhost";
	if(header == NULL){
		return NULL;
	}

	if(inFile != NULL){
		file = inFile;
	}

	if(inHost != NULL){
		host = inHost;
	}
	strcpy(header, "GET /");
	strcat(header, file);
	strcat(header, " HTTP/1.1\r\nHost:");
	strcat(header, host);
	strcat(header, "\r\nAccept: text/html\r\nConnection: Close\r\n\r\n");

	return header;
}

/**
 * Reads the data from sock and place only the HTML contents in the output file
 * Prints all of the contents read from sock
 */
int readSocket(int sock, FILE* output){
	char* buffer = calloc(128,sizeof(char)); //Make a buffer with clean memory


	char* start = NULL;            //Start and End of the HTML Document to avoid storing excess data
	char* end = NULL;

	int write = -1; 				//Don't start writing to file immediately
	int isOK = 0;				//Only enter loop if we have an html file denoted by an OK response


	do{
		free(buffer);
		buffer = calloc(128,sizeof(char)); //Make a buffer with clean memory

		int readStatus = read(sock, buffer, 127);   //Read into the buffer leaving a space at the end

		if(readStatus < 0){							//Error checking
			fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
			exit(0);
		}else{
			//		printf("Received Message ...\n");
		}

		buffer[127] = '\0'; 						//Make it a string to make processing easier
		printf("%s", buffer);						//Print the buffer so the server response is printed
		if(strstr(buffer,"200")){
			isOK = 1;
		}
	}while(!strstr(buffer, "\r\n\r\n"));			//Go through the entire header



	// Write to File
	while(end == NULL && isOK == 1){ 			//While the end of the HTML hasn't been seen






		end = strstr(buffer, "</html>");			//Look for ending tags
		if(end == NULL){
			end = strstr(buffer, "</HTML>");
		}

		start = strstr(buffer, "<html");		  	//Look for starting tags
		if(start == NULL){
			start = strstr(buffer, "<HTML");
		}
		if(start == NULL){
			start = strstr(buffer, "<!DOCTYPE ");
		}


		if(start != NULL){							//If start was in THIS buffer clear out all data up to start
			char* ptr = buffer;
			while(ptr < (start)){
				*ptr = ' ';
				ptr ++;
			}
			write = 0;								//Allow writing to the File
		}


		if(end != NULL){							//If the end was in THIS buffer end the string following the end tag
			end += 7;
			while(end < (buffer + 127)){
				*end = '\0';
				end ++;
			}
		}


		if(write == 0){								//If writing had been enabled write the buffer into the file
			//printf("%s", buffer);
			fputs(buffer,output);
		}


		free(buffer);								//Free the pointer
		buffer = calloc(127, sizeof(char));			//Get new clean data (TODO inefficient)


		int readStatus = read(sock, buffer, 127);   //Read into the buffer leaving a space at the end

		if(readStatus < 0){							//Error checking
			fprintf(stderr, "Error reading from socket: %s\n", strerror(errno));
			exit(0);
		}else{
			//		printf("Received Message ...\n");
		}

		buffer[127] = '\0'; 						//Make it a string to make processing easier
		printf("%s", buffer);

	}

	return 0;
}

