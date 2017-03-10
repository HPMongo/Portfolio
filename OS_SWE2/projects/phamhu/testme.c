/*
 * Author: Huy Pham
 * Created date: 01/29/2016
 * Program name: testme.c
 * Description: This program will demonstrate the random testing concept where
 * a random character will be generated and it will be checked against a shuffled
 * string. When the sequence of characters and conditions are met, the program will 
 * stop.
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

/* 
 * This function will generate a random printable character ASCII value 32 - 126
 */
char inputChar()
{
  	int randNum;
	char randChar;
	randNum = rand() % 95 + 32;	//printable characters	 
 	randChar = (char) randNum;
    return randChar;
}

/* 
 * This function will shuffle the string 'reset' in a random order and return it
 * to the calling function
 */
char *inputString()
{
    char *randString;
	char cString[] = "reset";
	char nString[6];
	int numArray[6];
	int randNum, i;
	// create a random array from 0 to 5
	for(i = 0; i < 6; i++)
	{
		int numCount;
		int j;
		int numNotFound = 1;
		while(numNotFound)
		{
			numCount = 0;				// reset counter
			randNum = rand() % 6;		// generate random number
			for(j = 0; j < i; j++)		// loop thru the num array
			{							// and count the number of match number 
				if(randNum == numArray[j])
					numCount++;
			}
			if(numCount == 0)
			{
				numNotFound = 0;		// number doesn't exist, get out of loop
			}
		}
		numArray[i] = randNum;			// set the random number in the array	
	}
	// shuffle the 'reset' string using the random array
	for (i = 0; i < 6; i++)
	{
		nString[i] = cString[numArray[i]];	
	}
	// assign shuffled string to char pointer
	randString = nString;
    return randString;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
