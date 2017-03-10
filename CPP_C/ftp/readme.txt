The ftp program is implemented using TCP protocol between a client and a server programs. The server program is written in C and the client program is written in Java. 

Before the FTP program can be used, both programs need to be compiled.
 For client program, use: 
	javac FtClient.java
For server program, use: 
	gcc -o ftserver ftserver.c

After the programs are compiled, FTP server program needs to be executed using a predefined port using: 
	ftserver [port]
This will let the server program listen to the <port_number>.

The client program can be connected to the server using either:
	java FtClient [host] [send_port_number] [-l] [receive_port_number]				// list contents of the directory
 	java FtClient [host] [send_port_number] [-g] [filename] [receive_port_number]	// get a file from the directory

To stop the server program, use Ctrl+C and the server will be terminated.

Extra credit:
	If the requested file already exists in the client directory, a message will be prompt to ask if the user wants to override the old file. If the user select 'Y', the old file will be replaced with the new file. Otherwise, the transfer will be canceled.
