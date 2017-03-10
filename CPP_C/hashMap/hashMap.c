/*
 * CS261 - Assignment 6 - hashMap.c
 * Name: Huy Pham
 * Date: 03/08/2015
 * Solution description: Hash Map implementation.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"



/*the first hashing function you can use*/
int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
	return r;
}

/*internal function - create index value by calling a hash function and divide the result by table size*/
int _getIdx(struct hashMap * ht, KeyType k)
{  
	assert(ht != 0);
	int hashIdx; 
	if(HASHING_FUNCTION == 1)
	{
//		printf("In hash1\n");
		hashIdx = stringHash1(k) % ht->tableSize;
	}
	else
	{
//		printf("In hash2\n");
		hashIdx = stringHash2(k) % ht->tableSize;
	}
	//adjust index
	if(hashIdx < 0)
		hashIdx += ht->tableSize;
	return hashIdx;
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
	assert(tableSize > 0);
	hashMap *ht;
	ht = malloc(sizeof(hashMap));
	assert(ht != 0);
	_initMap(ht, tableSize);
	return ht;
}

/*
 Free all memory used by the buckets.
 Note: Before freeing up a hashLink, free the memory occupied by key and value
 */
void _freeMap (struct hashMap * ht)
{  
	/*write this*/	
	struct hashLink *current, *next;
	for(int i = 0; i < ht->tableSize; ++i)
	{
		current = ht->table[i];
		while(current != 0)
		{
			next = current->next;
			//deallocate individual link
			free(current);
			current = next;
		}
	}	
	//deallocate table
	free(ht->table);
}

/* Deallocate buckets and the hash map.*/
void deleteMap(hashMap *ht) {
	assert(ht!= 0);
	/* Free all memory used by the buckets */
	_freeMap(ht);
	/* free the hashMap struct */
	free(ht);
}

/* 
Resizes the hash table to be the size newTableSize 
*/
void _setTableSize(struct hashMap * ht, int newTableSize)
{
	struct hashLink **oldHT = ht->table;
	//save old info
	int oldSize = ht->tableSize;
	//build existing table as new table
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * newTableSize);
	//reset count and size
	ht->count = 0;
	ht->tableSize = newTableSize;
	//initialize new table
	for(int i = 0; i < ht->tableSize; ++i)
	{
		ht->table[i] = NULL;
	}
	//add values to new table
	for(int i = 0; i < oldSize; ++i)
	{
		struct hashLink *currentLink = oldHT[i];
		struct hashLink *nextLink;
		while(currentLink != 0)
		{
			insertMap(ht, currentLink->key, currentLink->value);
			nextLink = currentLink->next;
			//delete old link
			free(currentLink);
			currentLink = nextLink;
		}
	}
	//delete old table
	free(oldHT);
}


/*
 insert the following values into a hashLink, you must create this hashLink but
 only after you confirm that this key does not already exist in the table. For example, you
 cannot have two hashLinks for the word "taco".
 
 if a hashLink already exists in the table for the key provided you should
 replace that hashLink--this requires freeing up the old memory pointed by hashLink->value
 and then pointing hashLink->value to value v.
 
 also, you must monitor the load factor and resize when the load factor is greater than
 or equal LOAD_FACTOR_THRESHOLD (defined in hashMap.h).
 */
void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{  
	//printf("-- In insertMap --\n");
	assert(ht != 0);
	int hashIdx = _getIdx(ht, k);

	if(containsKey(ht,k) == 1)
	{
		//printf("Key,%s, exists - updating value with %d at bucket[%d]\n", k, v, hashIdx);
		int *newVal = (int *)atMap(ht, k);
		assert(newVal != 0);
		*newVal = v;
	}
	else
	{
		//printf("Key,%s, does not exist - adding new record with value %d at bucket[%d]\n", k, v, hashIdx);	
		struct hashLink *newLink = malloc(sizeof(struct hashLink*));
		assert(newLink != 0);
		//Populate new link
		newLink->key = k;
		newLink->value = v;
		newLink->next = ht->table[hashIdx];
		//Add link to bucket
		ht->table[hashIdx] = newLink;
		//Increase size
		ht->count++;
	}
	//Check load factor and resize table if necessary
	if(tableLoad(ht) > LOAD_FACTOR_THRESHOLD)
	{
		_setTableSize(ht, ht->tableSize * 2);		//increase the size by 2
		//printf("New table size after-resized is: %d\n", ht->tableSize);
	}
}

/*
 this returns the value (which is void*) stored in a hashLink specified by the key k.
 
 if the user supplies the key "taco" you should find taco in the hashTable, then
 return the value member of the hashLink that represents taco.
 
 if the supplied key is not in the hashtable return NULL.
 */
ValueType* atMap (struct hashMap * ht, KeyType k)
{ 
	assert(ht != 0);
	int hashIdx = _getIdx(ht, k);
	struct hashLink *temp = ht->table[hashIdx];
	while(temp != 0)
	{
		if(strcmp(temp->key, k) == 0)
			return &temp->value;
		else
			temp = temp->next;
	}
	return NULL;
}

/*
 a simple yes/no if the key is in the hashtable. 
 0 is no, all other values are yes.
 */
int containsKey (struct hashMap * ht, KeyType k)
{  
	//printf("-- In containsKey --\n");
	assert(ht != 0);
	int hashIdx = _getIdx(ht, k);
	//printf("Searching in bucket[%d] for - %s -\n", hashIdx, k);
	struct hashLink *temp = ht->table[hashIdx];
	while(temp != 0)
	{
		if(strcmp(temp->key, k) ==0)
		{
	//		printf("Match found - current key, %s , is in bucket[%d] with value %d\n", temp->key, hashIdx, temp->value);
			return 1;	//key found
		}
		else
		{
	//		printf("No match - current key, %s , is at bucket[%d]\n", temp->key, hashIdx);
			temp = temp->next;
		}
	}	
	return 0;			//key not found
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.
 */
void removeKey (struct hashMap * ht, KeyType k)
{  
	assert(ht != 0);
	int hashIdx = _getIdx(ht, k);
	int pos = 0;
	struct hashLink *temp = ht->table[hashIdx];
	struct hashLink *prev = temp;
	while(temp != 0)
	{
		if(strcmp(temp->key, k) == 0)
		{
			//printf("In if - found key\n");
			//substract table size
			ht->count--;
			//reassign links
			if(pos == 0)		//the first link is removed from the bucket
			{			//reassign the head of the bucket to the next link
				ht->table[hashIdx] = temp->next;
			}	
			else
			{
				prev->next = temp->next;
			}
			//free link
			free(temp);
			temp = 0;
			printf("%s - has been removed!\n", k);
			return;	
		}
		else
		{
			pos++;
			//printf("In else - key not found\n");
			prev = temp;
			temp = temp->next;
		}
	}	
}

/*
 returns the number of hashLinks in the table
 */
int size (struct hashMap *ht)
{  
	assert(ht != 0);
	return ht->count;
}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht)
{  
	assert(ht != 0);
	return ht->tableSize;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht)
{  
	assert(ht != 0);
	int count = 0;
	for(int i = 0; i < ht->tableSize; ++i)
	{
		if(ht->table[i] == 0)
		{
			++count;
		}
	}
	return count;
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)
 
 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets (but remember hashlinks
 are like linked list nodes so they can hang from each other)
 */
float tableLoad(struct hashMap *ht)
{  
	assert(ht != 0);
	if(ht->tableSize == 0)
		return 0;
	else
		return ((float)ht->count/(float)ht->tableSize);
}
void printMap (struct hashMap * ht)
{
	int i;
	struct hashLink *temp;	
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
//		if(temp != 0) {		
//			printf("\nBucket Index [%d] -> ", i);		
//		}
		while(temp != 0){			
			printf("%s ", temp->key);
			printValue(temp->value);
//			printf(" -> ");
			printf("\n");
			temp=temp->next;			
		}		
	}
}


