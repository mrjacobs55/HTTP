/*
 * Client.h
 *
 *  Created on: Sep 3, 2019
 *      Author: mrjacobs
 */

#ifndef CLIENT_H_
#define CLIENT_H_

int main(int argc, char *argv[]);
char* makeV4(char* hostname);
char* createHeader(char* inFile, char* inHost, char* header);
int readSocket(int sock, FILE* output);

#endif /* CLIENT_H_ */
