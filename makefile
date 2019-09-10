
Server: Server/Server.o
	gcc -o http_server Server/Server.c
	
Client: Client/Client.o
	gcc -o http_client Client/Client.c
 	
all : 
	make Server
	make Client