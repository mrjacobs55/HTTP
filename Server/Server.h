
#ifndef SERVER_H_
#define SERVER_H_

int main(int argc, char *argv[]);
int get(char* name, int connection);
void sendHeader(int connection, int code);
void proccess(char* request, char* function, char* file);

#endif
