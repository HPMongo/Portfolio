/*
 * Author: Huy Pham
 * Created date: 02/06/2016
 * Program name: chatClient.c
 * Description: this is a client chat program. The program is using TCP protocol
 * to connect to a chat server on a predefined port.
 *
 * compile the program using: 
 *	gcc -o chatclient chatClient.c -Wall
 *
 * run the program using:
 * 	chatclient <port_number>
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_BUFF 500
/* This function will get the user's input for the client's handle name*/
void getHandle(char *result)
{
	printf("Please enter your handle name: ");
	scanf("%s", result);
//	printf("You entered: %s\n", result);	
}

/* This function checks if the user wants to quit*/
int isDone(char *input)
{
	if(strncmp(input, "\\quit", 5) == 0)
	{
		printf("Good bye!\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

/* This is the main logic of the program*/
int main (int argc, char *argv[])
{
	char handle[11];
	char buffer[MAX_BUFF];
	int quitProgram = 0;
	int portNum;
	int sockfd, n;
	struct sockaddr_in server_addr;
	struct hostent *server;
	
	// validate input arguments
	if(argc == 2)
	{
//		for(i = 0; i < argc; i++)
//		{
//			printf("[%d] - %s\n", i, argv[i]);
//		}
		// get port number
		portNum = atoi(argv[1]);
		if (portNum <= 0)
		{
			printf("%s is an invalid port\n", argv[2]);
			return 0;
		}
	}
	else
	{
		printf("Usage: chatclient  <port-number>\n");
		return 0;
	}

	// get client handle 
	getHandle(handle);

	// create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("Error opening socket from client\n");
		return 1;
	}
	
	// set values in buffer to zero
	bzero((char *) &server_addr, sizeof(server_addr));
	server = gethostbyname("access.engr.orst.edu");
	// set socket properties
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

	// connect socket to address
	if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		printf("Ops - it looks like there's a connection issue. Please try again.\n");
		return 1;
	}

	printf ("Connected to chat server\n");
	// start chat program
	do
	{
		bzero(buffer, MAX_BUFF);
			n = read(sockfd, buffer, MAX_BUFF);
			if (n < 0)
			{
//				printf("Error - reading from the socket - client\n");
				quitProgram = 1;	
			}
			else
			{
				printf("%s\n", buffer);
			}
	
		bzero(buffer, MAX_BUFF);
	
		printf("%s>", handle);
		fgets(buffer, 500, stdin);
//		printf("%s>%s\n", handle, buffer);
		quitProgram = isDone(buffer);
		// write to socket
		if(quitProgram == 0)
		{
			n = write(sockfd, buffer, strlen(buffer));
			if (n < 0)
			{
				printf("Error - writing to the socket from client\n");
				quitProgram = 1;;
			}			
//			else
//			{
//				printf("Wrote %d bytes to the server\n", n);
//			}	
}
	} while (quitProgram == 0);

	// close socket
	close(sockfd);
	return 0;
}
