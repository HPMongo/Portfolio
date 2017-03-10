/*
 * CS261 - Assignment 6 - main.c
 * Name: Huy Pham
 * Date: 03/08/2015
 * Solution description: using hash map data structure to create a hash table.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashMap.h"

/*
 the getWord function takes a FILE pointer and returns you a string which was
 the next word in the in the file. words are defined (by this function) to be
 characters or numbers seperated by periods, spaces, or newlines.
 
 when there are no more words in the input file this function will return NULL.
 
 this function will malloc some memory for the char* it returns. it is your job
 to free this memory when you no longer need it.
 */
char* getWord(FILE *file);

int main (int argc, const char * argv[]) {
	const char* filename;
	struct hashMap *hashTable;	
	int tableSize = 1009;
	int fileOpen = 0;
	clock_t timer;
	FILE *fileptr;	
    /*
     this part is using command line arguments, you can use them if you wish
     but it is not required. DO NOT remove this code though, we will use it for
     testing your program.
     
     if you wish not to use command line arguments manually type in your
     filename and path in the else case.
     */
    	if(argc == 2)
        	filename = argv[1];
    	else
        	filename = "input1.txt"; /*specify your input text file here*/
    
   	printf("opening file: %s\n", filename);
    
	timer = clock();
	
	hashTable = createMap(tableSize);	   
	
    	/*... concordance code goes here ...*/
	fileptr = fopen(filename, "r");

	if(fileptr == 0)
	{
		printf("Error - unable to open the input file.");
		return 1;
	}	
	else
	{
		fileOpen = 1;	//set file open to true
		char *key = getWord(fileptr);
		while(key != 0)
		{
			//printf("* * Adding new word -%s - * *\n", key);
			if(containsKey(hashTable,key) == 0)
			{
			//	printf("--Key '%s' not in the table - insert new record\n", key);
				insertMap(hashTable, key, 1);
			} 
			else
			{
			//	printf("--Key '%s' is in the table - update record\n", key);
				//retriev value	
				int *tempVal = (int*)atMap(hashTable, key);
				assert(tempVal != 0);
				insertMap(hashTable, key, *tempVal + 1);
			}
			key = getWord(fileptr);
		}
	}
	/*... concordance code ends here ...*/
	printf("\n");
	printMap(hashTable);
	timer = clock() - timer;
	printf("\nconcordance ran in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
	printf("Table emptyBuckets = %d\n", emptyBuckets(hashTable));
    	printf("Table count = %d\n", size(hashTable));
	printf("Table capacity = %d\n", capacity(hashTable));
	printf("Table load = %f\n", tableLoad(hashTable));
	
	printf("Deleting keys\n");
	
	removeKey(hashTable, "and");
	removeKey(hashTable, "me");
	removeKey(hashTable, "the");
	removeKey(hashTable, "J");
//	printMap(hashTable);

	printf("\nTable emptyBuckets = %d\n", emptyBuckets(hashTable));
    	printf("Table count = %d\n", size(hashTable));
	printf("Table capacity = %d\n", capacity(hashTable));
	printf("Table load = %f\n", tableLoad(hashTable));
		
	deleteMap(hashTable);
	printf("\nDeleted the table\n"); 
	if(fileOpen == 1)		//file was open
		fclose(fileptr);  	//close the file
	return 0;
}

void printValue(ValueType v) {
	printf(":%d",(int *)v);
}

char* getWord(FILE *file)
{
	int length = 0;
	int maxLength = 16;
	char character;
    
	char* word = malloc(sizeof(char) * maxLength);
	assert(word != NULL);
    
	while( (character = fgetc(file)) != EOF)
	{
		if((length+1) > maxLength)
		{
			maxLength *= 2;
			word = (char*)realloc(word, maxLength);
		}
		if((character >= '0' && character <= '9') || /*is a number*/
		   (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
		   (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
		   character == 39) /*or is an apostrophy*/
		{
			word[length] = character;
			length++;
		}
		else if(length > 0)
			break;
	}
    
	if(length == 0)
	{
		free(word);
		return NULL;
	}
	word[length] = '\0';
	return word;
}
