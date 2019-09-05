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


int main(int argc, char *argv[]){
	int sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0){
		printf("Socket Creation Failed\n");
		exit(0);
	}else{
		printf("Socket opened...\n");

	}






	return 1;
}

