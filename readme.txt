Author: Mitchell Jacobs
Username: mrjacobs

Implementation of the HTTP protocol in the C programming language.

This contains both a server application and a client application.

To Compile the Server:
	make Server
		or
	gcc -o http_server Server/Server.c
	
To Compile the Client:
	make Client
		or
	gcc -o http_client Client/Client.c
	
To Compile both Simultaneously:
	make all
	
Running the Client:
	./http_client [-options] server_url port_number
	
		[-options] - OPTIONAL 
					-p prints the RTT for the server before the server response
					all others - ignored
					only one option may be specified
					
		server_url - The url or ip address of the server followed by a "/" and 
					 the file to retrieve from the server. The file and "/" are
					 optional and will default to "index.html" if left out.
					 The server address will default to localhost if forgotten
					 
		port_number - The port on which to connect to the server. 
					  If forgotten or invalid will default to 8080
	
	Examples:
		./http_client -p mit.edu/index.html 80
		./http_client mit.edu 80
		./http_client www.google.com/index.html 80
		./http_client localhost/index.html 12452	
	
	Output:
		If -p is specified 	the RTT will be printed on the terminal in MilliSeconds  
		The entire Server Response will be printed in the terminal
		If the server serves an HTML document and a HTTP response with code 200 it will be saved 
		under output.html 
		The program will automatically terminate upon receiving a response from the server
		
Running the Server:
	./http_server port_number
	
		port_number - The port on which to connect to the server. 
					  If forgotten or invalid will default to 8080.			
					 
	Examples:
		./http_server 80
		./http_server 12452
		
	Output
		The server will print that it is running
		Should the server receive a request for a file it does not have 
		it will print that it sent a 404 response and the file it was requested
		
Note:
	The CCC server does not seem to be able to be accessed externally so testing was 
	limited to localhost and on different platforms.
					  
					  
					  
					  
					  
					  
					  
				