/*
 * CS261 - assignment 6 - spellcheck.c
 * Name: Huy Pham
 * Date: 03/08/2015
 * Solution description: This is a spell check program using hash map as the 
 * underline data structure. If the user does not provide a dictionary input, 
 * the default input, dictionary.txt, will be used as the base input. 
 * Once the dictionary is loaded, the user will be able to enter in single word 
 * and the program will display whether it is a valid or invalid word. This will 
 * continue until the user decides to quit - type 'quit'.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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

/*
 Load the contents of file into hashmap ht
 */
void loadDictionary(FILE* file, struct hashMap* ht);

int main (int argc, const char * argv[]) {
  	clock_t timer;
  	struct hashMap* hashTable;
  	int tableSize = 1000;
  	const char* filename;
  	timer = clock();
  	FILE* dictionary;
  	hashTable = createMap(tableSize);
 
  	if(argc == 2)
        	filename = argv[1];
  	else
        	filename = "dictionary.txt"; /*default dictionary input*/

	dictionary = fopen(filename, "r");
       	if(dictionary == 0)
        {
                printf("Error - unable to open the input file.");
                return 1;	//set file open to false
        }
        else
        {
 		loadDictionary(dictionary,hashTable);
  		timer = clock() - timer;
		printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
  
  		char* word = (char*)malloc(256*sizeof(char));
  		int quit=0;
  		while(!quit){
    			printf("Enter a word to check spelling (type 'quit' when you want to exit): ");
    			scanf("%s",word);
   
   		//   ... spell checker code goes here ...
   			if(containsKey(hashTable, word) == 0)
			{
				printf("%s is not a valid word! Please try again!\n", word);
			}
			else
			{
				printf("%s is a valid word! Way to go!\n", word);
			}	
   		//   ... end of spell checker      ...
   
    
    		/* exit the loop when the user types 'quit'*/
    			if(strcmp(word,"quit")==0)
      				quit=!quit;
  		}
  		free(word);
  		deleteMap(hashTable);
  		printf("\nDeleted the dictionary.\n");
		fclose(dictionary); 
		printf("Good-bye!!\n");
	}
  	return 0;
}

void loadDictionary(FILE* file, struct hashMap* ht)
{
         char *key = getWord(file);
         while(key != 0)
         {
                if(containsKey(ht,key) == 0)
                {
                    	insertMap(ht, key, 1);
		}
		else
		{
			int *tempVal = (int*)atMap(ht, key);
			assert(tempVal != 0);
			insertMap(ht, key, *tempVal +1);
		}
		key = getWord(file);
	}
}

char* getWord(FILE *file)
{
	int length = 0;
	int maxLength = 16;
	char character;
    
	char* word = (char*)malloc(sizeof(char) * maxLength);
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
		   (character == 39)) /*or is an apostrophy*/
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

void printValue(ValueType v) {
	printf(":%d", (int*)v);
}
