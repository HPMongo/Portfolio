/*
 * Program: keygen.c
 * Author: Huy Pham
 * Written on: 12/1/2015
 * Description: This program will generate a random list of characters based on a given 
 * length.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{
	int i, c, length;
	char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	srand(time(NULL));
	char *p;
	if(argc > 1)
	{
		long conv = strtol(argv[1], &p, 10);
		length = conv;
		for (i = 0; i < length; i++)
		{
			c = rand() % 27;	//generate a random integer between 0-26
			printf("%c", alpha[c]);
		}
		printf("\n");
	}
	return 0;
}

