/*
 * Program: otp_dec.c
 * Author: Huy Pham
 * Written on: 12/1/2015
 * Description: This program will send a text file to a predefined port
 * to decrypt the content of a file using a one-time key pad.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define MAX_BUFF 200000
/*
 * This function will send the combined plain text and key message to the
 * server for decryption. Result of the decryption will be displayed back
 * on the console.
 */
int sendToServer(char *argv[], char tFile[])
{
	int rCode = 0;
	int sockfd, n, portno;
	struct sockaddr_in server_addr;
	struct hostent *server;
	char buffer[MAX_BUFF];
	FILE *fp1;
	// get port number from the argument
	portno = atoi(argv[3]);

	// create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("Error opening socket from client\n");
		rCode = 1;
	}

	// set values in buffer to zero
	bzero((char *) &server_addr, sizeof(server_addr));
	server = gethostbyname("eos-class.engr.oregonstate.edu");
	// set socket properties
	server_addr.sin_family = AF_INET;
	server_addr.sin_port =  htons(portno); 
	bcopy((char *)server->h_addr, 
		  (char *)&server_addr.sin_addr.s_addr,	
		  server->h_length);
	
	// connect socket to address
	if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		printf("Error connecting from the client\n");
		rCode = 1;
	}
	
///	printf("Client is ready for transmission - sending plaintext\n");
	bzero(buffer, MAX_BUFF);

//	fp1 = fopen(argv[1], "r");	
    fp1 = fopen(tFile, "r");
	if(fp1 == NULL)
	{
		printf("In client - error opening plaintext file\n");
		rCode = 1;
		return(-1);
	}
	
	if(fgets(buffer, MAX_BUFF, fp1) != NULL)
	{
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			printf("Error writing to the socket - client\n");
			rCode = 1;
		}
///		else
///		{
///			printf("Wrote %d bytes to server - plain\n", n);
///		}
		bzero(buffer, MAX_BUFF);
		n = read(sockfd, buffer, MAX_BUFF);
		if (n < 0)
		{
///			printf("Error reading from the socket -client\n");
			rCode = 1;
		}
		else		// print decrypted message from server
		{
			printf("%s\n", buffer);
		}
	}
	fclose(fp1);

	// close socket
	close(sockfd);

	return rCode;
}

/*
 * This function will merge the contents of file1 and file2 into file3.
 */
void createFile(char file1[], char file2[], char file3[])
{
	FILE *fp1, *fp2, *fp3;
	char buffer[MAX_BUFF];
	fp1 = fopen(file1, "r");
	fp2 = fopen(file2, "r");
	fp3 = fopen(file3, "w+");
	if (fp3 == NULL)
	{
		printf("Error -open %s-", file3);
		exit(1);	
	}

	if (fp1 == NULL)
	{
		printf("Error -open %s-", file1);
		exit(1);	
	}
	else
	{
		if(fgets(buffer, MAX_BUFF, fp1) != NULL)
		{
			fputs(buffer, fp3);
			fseek(fp3, -1, SEEK_END);
			fputs("<", fp3);
			fclose(fp1);
		}	
	}

	if (fp2 == NULL)
	{
		printf("Error -open %s-", file2);
		exit(1);	
	}
	else
	{
		bzero(buffer, MAX_BUFF);
		if(fgets(buffer, MAX_BUFF, fp2) != NULL)
		{
			fseek(fp3, 0, SEEK_END);
			fputs(buffer, fp3);
			fseek(fp3, -1, SEEK_END);
			fputs("<", fp3);
			fclose(fp2);
			fclose(fp3);
		}
	}
}

/*
 * This function will validate the arguments passed into the program.
 */
int validateInput(char *argv[], int argc)
{
	int rCode = 0, size1 = 0, size2 = 0;
	FILE *fp1, *fp2;
	// validate plain text file
	fp1 = fopen(argv[1], "r");
	if (fp1 != NULL)
	{
		fseek(fp1, 0, SEEK_END);	//seek to the end of file
		size1 = ftell(fp1);			//get position for eof
		fseek(fp1, 0, SEEK_SET);	//reset position to 0
		fclose(fp1);
	}
	else
	{
		printf("Plain text file -%s- does not exist\n", argv[1]);
		rCode = 1;
	}

	// validate key file
	if (rCode == 0)
	{
		fp2 = fopen(argv[2], "r");
		if(fp2 != NULL)
		{
			fseek(fp2, 0, SEEK_END);	//seek to the eof
			size2 = ftell(fp2);			//get position for eof
			fseek(fp2, 0, SEEK_SET);	//reset position to 0
			fclose(fp2);
		}
		else
		{
			printf("Key text file -%s- does not exist\n", argv[2]);
			rCode = 1;
		}	
	}

	// compare two files
	if (rCode == 0)
	{
		if(size1 > size2)
		{
			printf("Key '%s' is too short\n", argv[2]);
			rCode = 1;
		}
	}

	// check for valid port
	if (rCode == 0)
	{
		int portno;
		portno = atoi(argv[3]);
		if (portno <= 0)
		{
			printf("%s is an invalid port\n", argv[3]);
			rCode = 2;
		}		
	}
	return rCode;
}

/*
 * This is the main logic of the program
 */
int main(int argc, char *argv[])
{
	int pid = getpid();
	int rc = 0, rm = 0;
	char tFile[24] = "tempFile";	
	// check number of argument provided
	if (argc < 4)
	{
		printf("Not enough arguments to run the program\n");
		exit(0);
	}
	else		//send inputs for validation
	{
		rc = validateInput(argv, argc);
		if(rc != 0)
		{
			if(rc == 2)
				exit(2);
			else
				exit(1);
		}
		else
		{
			char sPID[10];
			sprintf(sPID, "%d", pid);
			strcat(tFile,sPID);
			createFile(argv[1], argv[2], tFile);
			int result;
			result = sendToServer(argv, tFile);
			if (result != 0)
			{
///				printf("Encountered error sending to server\n");
				exit(1);
			}
			rm = remove(tFile);
			if(rm != 0)
			{
				printf("Error - unable to delete %s\n", tFile);
			}
		}	
	}
	return 0;
}
