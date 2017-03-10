The chat program is implemented using TCP protocol between a client and a server programs. The server program is written in Java and the client program is written in C. For demonstration purposes, both programs need to reside on the same server.

Before the chat program can be used, both programs need to be compiled.
For server program, use: 
	javac ChatServer.java
For client program, use: 
	gcc -o chatclient chatClient.c -Wall

After the programs are compiled, chat server program needs to be executed using a predefined port using: 
	java ChatServer <port_number> 
This will let the server program listen to the <port_number>.

The client program can be connected to the server using: 
	chatclient <port_number>.

Note: due to the various performance on the host, it could take a few tries to connect to the server. 

Once connection is establish, client and server can exchange messages.Whenever the user wants to stop the program, the user can type '\quit' and the program will be terminated.
