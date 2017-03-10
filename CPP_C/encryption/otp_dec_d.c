/*
 * Program: otp_dec_d.c
 * Author: Huy Pham
 * Written on: 12/1/2015
 * Description: This program will decrypt a text file using one-time-pad provided
 * by the calling program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define MAX_BUFF 200000

/*
 * This function will separate the key from the plain text buffer and 
 * return the starting position of the delimiter.
 */
int copyKey(char buffer[], char key[])
{
	int keyStart = 0;
	char * pch;
///	printf("Search for '<' in buffer\n");
	pch=strchr(buffer,'<');
	if(pch != NULL)
	{
		keyStart = pch - buffer;
///		printf("Found '<' at %d\n", keyStart);
		// copy key to key array
		int i, startPos;
		startPos = keyStart + 1;
		for (i = 0; i < keyStart; i++)
		{
			key[i] = buffer[startPos];
			startPos++;
		}
		key[keyStart] = '\0';	//set string terminating 
///		printf("Extracted key: %s-\n", key);
	}
	else
	{
		printf("Key delimiter is not found\n");
		keyStart = -1;
	}
	return keyStart;
}

/*
 * Helper function to convert char to int
 */
int charToInt(char c)
{
	if (c == ' ')
	{
		return 26;
	}
	else
	{
		return (c - 'A');
	}
}

/*
 * Helper function to convert int to char
 */
char intToChar(int i)
{
	if (i == 26)
	{
		return ' ';
	}
	else
	{
		return (i + 'A');
	}
}

/*
 * This function will decrypt the plain text message using the key.
 * The result of the decryption will be stored in the decryptedMsg array
 * and the number of decrypted bytes will be returned to the calling 
 * function.
 */
int decrypt(char buffer[],char key[], char decryptedMsg[],int keyStart)
{
	int byteDecrypted = 0, i, diff;
	for (i = 0; i < keyStart; i++)
	{
		diff = charToInt(buffer[i])- charToInt(key[i]);
		if(diff < 0)
		{
			diff = diff + 27;	
		}
		diff = diff % 27;
		decryptedMsg[i] = intToChar(diff);	
		byteDecrypted++;
	}		
	return byteDecrypted;
}	
	
void dostuff(int sockfd)
{
	int n, keyStart, byteDecrypted;
	char buffer[MAX_BUFF];
	char key[MAX_BUFF];
	char decryptedMsg[MAX_BUFF];
	// set values in buffer to zero
	bzero(buffer, MAX_BUFF);
	n = read(sockfd, buffer, MAX_BUFF);
	if (n < 0)
	{
		printf("Error on reading from the socket\n");
	}
///	else
///	{
///		printf("Server - read %d bytes from socket %d\n", n, sockfd);
///	}
	// print from buffer
///	printf("Here is the message: %s\n", buffer);

	// separate key from plain text
	keyStart = copyKey(buffer, key);

	// doing decryption
	if (keyStart > 0)
	{
		byteDecrypted = decrypt(buffer, key, decryptedMsg, keyStart);		
		if (byteDecrypted > 0)
		{
///			printf("Decrypted text -%s-\n", decryptedMsg);
			// write decrypted message back to the client
			n = write(sockfd, decryptedMsg,MAX_BUFF);
			if (n < 0)
			{
				printf("Error on writing back to the client\n");
			}
///			else
///			{
///			printf("Server - wrote %d bytes to client\n", n);
///			}
		}
	}	
}

/*
 * This is the main logic of the program.
 */
int main (int argc, char *argv[])
{
	int port, sockfd, newsockfd, pid, status;
	struct sockaddr_in server, client;
	socklen_t client_length;
	if(argc > 1)
	{
///		printf("Argc: %d\n", argc);
 	   	port = atoi(argv[1]);
///		printf("port: %d\n", port);		
	}
	else
	{
		printf("No port provided!\n");
		exit(1);
	}
	// create socket	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		printf("Encounter error opening socket!\n");
		exit(1);
	}

	// set values in buffer to zero
	bzero((char *) &server, sizeof(server));

	// set socket struc properties 
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to an address
	if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		printf("Error on binding\n");
		exit(1);
	}

	// listen on the socket for connections and set backlog queue to 5
///	printf("Start listening on %d\n", port);
	listen(sockfd, 5);
	
	// create a block until the client connects to the server
	client_length = sizeof(client);

	while(1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &client, &client_length); 
		if (newsockfd < 0)
		{
			printf("Error on accepting connection\n");
		}
		pid = fork();
		if (pid < 0)
		{
			printf("Error on fork\n");
		}
		if (pid == 0)
		{
			close(sockfd);
			dostuff(newsockfd);
			exit(0);
		}
		else
		{
		// close socket
			close(newsockfd);
		}

		// wait for child process		
		while(pid > 0)
		{
			pid = waitpid(-1, &status, WNOHANG);
		}
	}
	close(sockfd);
	return 0;	// never get here
}

