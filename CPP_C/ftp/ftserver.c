/*
 * Author: Huy Pham
 * Created date: 3/1/2016
 * Program name: ftserver.c
 * Description: This is server file transfer program. The program is using TCP protocol and it listens
 * to a predefined port.
 *
 * Compile the program using: 
 * 	gcc -o ftserver ftserver.c
 * Run program using: 
 * 	ftserver [port]
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_BUFF 200000

/*
 * This function will search to see if the requested file exists. If it exists, the file
 * will be read and written to the socket. If it doesn't, an error message will be sent
 * back to the socket.
 *
 * Credits:
 * (1) - http://www.cplusplus.com/reference/cstdio/fread/
 */
void getFile(int sockfd, char host[], int outPort, char fileName[])
{
	int n;
	FILE * fp;
	char buffer[MAX_BUFF];
	
	fp = fopen(fileName, "r");
	if (fp != NULL)
	{
		printf("Sending '%s' to %s:%d\n", fileName, host, outPort);
		// write success return code to socket
		n = write(sockfd, "1|", 2);

		// (1) //
		fseek (fp, 0, SEEK_END);
		long lSize = ftell(fp);
		printf("File size is: %lu bytes\n", lSize);
		rewind(fp);
		char * buffer1;
		size_t result;
		buffer1 = (char*) malloc (sizeof(char)*lSize);
		if(buffer1 != NULL)
		{
			result = fread(buffer1, 1, lSize, fp);
			if(result != lSize)
			{
				printf("Error - reading from file\n");
			}	
			else
			{
				int byte_read = result;
				void *p = buffer1;
				while (byte_read > 0)
				{
					n = write(sockfd, p, byte_read);
					printf("Writing %d bytes to socket\n", n);
					if(n <= 0)
					{
						printf("Nothing left to read\n");
					}
					byte_read -= n;
					p += n;
				}	
			}
		}
		free(buffer1);
		strcpy(buffer, "|");
		n = write(sockfd, buffer, strlen(buffer));
		// close file
		fclose(fp);
	}
	else
	{
		printf("File not found! Sending error message to %s:%d\n", host, outPort);
		// write not found return code and message back to socket
		strcpy(buffer, "100|FILE NOT FOUND|");
		n = write(sockfd, buffer, strlen(buffer)); 
	}
}
/* This function will list all of the files in the current directory
 * 
 * Credits:
 * (1) - http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
 */
void listDir(int sockfd, char host[], int outPort)
{
	int n;
	char currentDir[200];
	char outPut[MAX_BUFF];
	// (1) //
	DIR		*d;						/* the directory */
	struct dirent *dir = NULL;		/* each entry	 */
	// get current directory
	getcwd(currentDir, 200);
	// open directory
	d = opendir(currentDir);
 	if (d)
	{
		// read content from directory
		while((dir = readdir(d)) != NULL)
		{
			if((strcmp(dir->d_name,".") != 0) && (strcmp(dir->d_name,"..") != 0))
			{
				strcat(outPut, dir->d_name);
				strcat(outPut, "|");		
			}
		}
		closedir(d);
	}
	printf("Sending directory contents to %s:%d\n", host, outPort);
	n = write(sockfd, outPut, strlen(outPut));
}
	
void dostuff(int sockfd)
{
	int n;
	int inCmd;
	int outPort;
	char buffer[MAX_BUFF];
	char tempString[50];
	char requestedHost[50];
	char fileName[50];
	char * pch;
	// set values in buffer to zero
	bzero(buffer, MAX_BUFF);

	n = read(sockfd, buffer, sizeof(buffer));
	if (n < 0)
	{
		printf("Error on reading from the socket\n");
	}

	// parse the request
	pch = strtok(buffer, "|");
	inCmd = atoi(pch);
	pch = strtok(NULL, "|");
	strcpy(tempString, pch);
	pch = strtok(NULL, "|");
	outPort = atoi(pch);
	pch = strtok(NULL, "|");
	strcpy(fileName, pch);
	pch = strtok(tempString, ".");
	strcpy(requestedHost,pch);

	strcpy(tempString, fileName);
	pch = strtok(tempString, "|");
	strcpy(fileName, pch);

	printf("Connected to %s\n", requestedHost);

	if (inCmd == 1)
	{
		printf("List directory requested on port %d\n", outPort);
		listDir(sockfd, requestedHost, outPort);
	}
	else if (inCmd == 2)
	{
		printf("File '%s' requested on port %d.\n", fileName, outPort);
		getFile(sockfd, requestedHost, outPort, fileName);
	}
	else
	{
		printf("Good banana! How did you get here?\n");
	}
}

int main (int argc, char *argv[])
{
	int port, sockfd, newsockfd, pid, status;
	struct sockaddr_in server, client;
	socklen_t client_length;
	if(argc > 1)
	{
   	 	port = atoi(argv[1]);
	}
	else
	{
		printf("Usage: ftserver [port]\n");
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
	printf("Server open on %d\n", port);
	listen(sockfd, 5);
	
	// create a block until the client connects to the server
	client_length = sizeof(client);

	while(1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &client, &client_length); 
		if (newsockfd < 0)
		{
			printf("Error on accepting connection\n");
		}
		else
		{
			printf("\nConnection established\n");
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
