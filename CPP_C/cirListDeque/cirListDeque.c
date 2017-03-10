/* CS261 - Assignment 3- cirListDeque.c
 * Name: Huy Pham
 * Date: 02/01/2015
 * Solution description: implementation of a circular linked list for the deque ADT.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "cirListDeque.h"

/* Double Link Struture */
struct DLink {
	TYPE value;/* value of the link */
	struct DLink * next;/* pointer to the next link */
	struct DLink * prev;/* pointer to the previous link */
};

# define TYPE_SENTINEL_VALUE DBL_MAX 


/* ************************************************************************
 Deque ADT based on Circularly-Doubly-Linked List WITH Sentinel
 ************************************************************************ */

struct cirListDeque {
	int size;/* number of links in the deque */
	struct DLink *Sentinel;	/* pointer to the sentinel */
};
/* internal functions prototypes */
struct DLink* _createLink (TYPE val);
void _addLinkAfter(struct cirListDeque *q, struct DLink *lnk, TYPE v);
void _removeLink(struct cirListDeque *q, struct DLink *lnk);



/* ************************************************************************
	Deque Functions
************************************************************************ */

/* Initialize deque.

	param: 	q		pointer to the deque
	pre:	q is not null
	post:	q->Sentinel is allocated and q->size equals zero
*/
void _initCirListDeque (struct cirListDeque *q) 
{
	assert(q != 0);
	q->Sentinel = malloc(sizeof(struct DLink));
	assert(q->Sentinel != 0);
	q->Sentinel->next = 0;
	q->Sentinel->prev = 0;
	q->size = 0;
//	printf("In initCirListDeque\n");
}

/*
 create a new circular list deque
 
 */

struct cirListDeque *createCirListDeque()
{
	struct cirListDeque *newCL = malloc(sizeof(struct cirListDeque));
	_initCirListDeque(newCL);
	return(newCL);
}


/* Create a link for a value.

	param: 	val		the value to create a link for
	pre:	none
	post:	a link to store the value
*/
struct DLink * _createLink (TYPE val)
{
//	printf("In createLink\n");
	struct DLink *newLink = malloc(sizeof(struct DLink));
	assert(newLink != 0);
	newLink->value = val;
	newLink->next = 0;
	newLink->prev = 0;
//	printf("Created successful\n");
	return(newLink);	 
}

/* Adds a link after another link

	param: 	q		pointer to the deque
	param: 	lnk		pointer to the existing link in the deque
	param: 	v		value of the new link to be added after the existing link
	pre:	q is not null
	pre: 	lnk is not null
	pre:	lnk is in the deque 
	post:	the new link is added into the deque after the existing link
*/
void _addLinkAfter(struct cirListDeque *q, struct DLink *lnk, TYPE v)
{
	assert(q != 0);
	assert(lnk != 0);
	int linkFound = 0;
	struct DLink *newLink = malloc(sizeof(struct DLink));
	assert(newLink != 0);
	struct DLink *startingLink = malloc(sizeof(struct DLink));
	assert(startingLink != 0);
	startingLink = q->Sentinel->next; 
//check that lnk exists in the list
	while(startingLink)
	{
		if(startingLink == lnk)
		{
			linkFound = 1;		//set switch to true
			startingLink = 0;	//set loop termination to true
		}
		else
		{
			startingLink = startingLink->next;	//set link to next link
		}
	}
//check link condition
	if(linkFound)
	{
		if(lnk == q->Sentinel->prev)		//the given link is at the end of the list
		{					//
			addBackCirListDeque(q, v);	//call addBack function to add v to the list
		}
		else					//the given link is not at the end of the list
		{					//
			newLink->value = v;		//add data to the new link and reconnect existing links
			lnk->next->prev = newLink;
			newLink->next = lnk->next;
			newLink->prev = lnk;
			lnk->next = newLink;
		}		
	}
	else
	{
		printf("Unable to add the new link as the given link is invalid.\n");
	}
	free(startingLink);
}

/* Adds a link to the back of the deque

	param: 	q		pointer to the deque
	param: 	val		value for the link to be added
	pre:	q is not null
	post:	a link storing val is added to the back of the deque
*/
void addBackCirListDeque (struct cirListDeque *q, TYPE val) 
{
//	printf("In addBack\n");
	assert(q != 0);
	struct DLink *newLink = _createLink(val);
	newLink->next = 0;
	newLink->prev = q->Sentinel->prev;
	if(q->Sentinel->prev == 0)		//nothing has been added to the back
	{					//
		q->Sentinel->prev = newLink;	//assign the new link to the back sentinel
		q->Sentinel->next = newLink;
	}
	else						//previous link exists
	{						//
		q->Sentinel->prev->next = newLink;	//assign previous link next to the new link
		q->Sentinel->prev = newLink;		//assign the new link as the back sentinel
	}
	q->size += 1;
//	printf("Add successful!\n");
}

/* Adds a link to the front of the deque

	param: 	q		pointer to the deque
	param: 	val		value for the link to be added
	pre:	q is not null
	post:	a link storing val is added to the front of the deque
*/
void addFrontCirListDeque(struct cirListDeque *q, TYPE val)
{
//	printf("In addFront\n");
	assert(q != 0);
	struct DLink *newLink = _createLink(val);
	newLink->prev = 0;
	newLink->next = q->Sentinel->next;
	if(q->Sentinel->next == 0)		//nothing has been added to the front
	{					//
		q->Sentinel->next = newLink;	//assign the new link to the front sentinel
		q->Sentinel->prev = newLink;	
	}
	else						//next link exists
	{						//
		q->Sentinel->next->prev = newLink;	//assign next link prev to new link
		q->Sentinel->next = newLink;		//assign the new link to the front sentinel
	}
	q->size += 1;
//	printf("Add successful!\n");
}

/* Get the value of the front of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	none
	ret: 	value of the front of the deque
*/
TYPE frontCirListDeque(struct cirListDeque *q) 
{
	assert(q != 0);
	assert(q->size > 0);
	return(q->Sentinel->next->value);
}

/* Get the value of the back of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	none
	ret: 	value of the back of the deque
*/
TYPE backCirListDeque(struct cirListDeque *q)
{
	assert(q != 0);
	assert(q->size > 0);
	return(q->Sentinel->prev->value);
}

/* Remove a link from the deque

	param: 	q		pointer to the deque
	param: 	lnk		pointer to the link to be removed
	pre:	q is not null and q is not empty
	post:	the link is removed from the deque
*/
void _removeLink(struct cirListDeque *q, struct DLink *lnk)
{
	assert(q != 0);
	assert(q->size > 0);
	assert(lnk != 0);
	lnk->next->prev = lnk->prev;
	lnk->prev->next = lnk->next;
	lnk->next = 0;
	lnk->prev = 0;
	free(lnk);
	q->size -= 1;
}

/* Remove the front of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	the front is removed from the deque
*/
void removeFrontCirListDeque (struct cirListDeque *q) {
	assert(q != 0);
	assert(q->size > 0);
	q->Sentinel->next->next->prev = 0;	
	q->Sentinel->next = q->Sentinel->next->next;
	q->size -= 1;
}


/* Remove the back of the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post:	the back is removed from the deque
*/
void removeBackCirListDeque(struct cirListDeque *q)
{
	assert(q != 0);
	assert(q->size > 0);
	q->Sentinel->prev->prev->next = 0;
	q->Sentinel->prev = q->Sentinel->prev->prev;
	q->size -= 1;
}

/* De-allocate all links of the deque

	param: 	q		pointer to the deque
	pre:	none
	post:	All links (including Sentinel) are de-allocated
*/
void freeCirListDeque(struct cirListDeque *q)
{
	struct DLink *nextLink = malloc(sizeof(struct DLink));
	assert(nextLink != 0);
	nextLink = q->Sentinel->next;
	while(nextLink)
	{
		free(nextLink->prev);
		nextLink = nextLink->next;
	}
	free(nextLink);
	printf("Memory deallocated!\n");
}

/* Check whether the deque is empty

	param: 	q		pointer to the deque
	pre:	q is not null
	ret: 	1 if the deque is empty. Otherwise, 0.
*/
int isEmptyCirListDeque(struct cirListDeque *q) {
	assert(q != 0);
	if(q->size > 0)	
	{
		return 0;	//not empty
	}
	else
	{
		return 1;	//empty
	}
}

/* Print the links in the deque from front to back

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post: 	the links in the deque are printed from front to back
*/
void printCirListDeque(struct cirListDeque *q)
{
//	printf("Printing the entire linked list!\n");
	char *printType;
	int itemsRemain = 0;
	assert(q != 0);
	assert(q->size > 0);
	struct DLink *startingLink = malloc(sizeof(struct DLink));
	assert(startingLink != 0);
	startingLink = q->Sentinel->next;
//	printf("Starting pointer: %p \n", startingLink);
	itemsRemain = q->size;
//Test for TYPE data
	if(sizeof(startingLink->value) == sizeof(int))
	{
		printType = "%d\n";
	}
	else if(sizeof(startingLink->value) == sizeof(float))
	{
		printType = "%f\n";
	}
	else if(sizeof(startingLink->value) == sizeof(double))
	{
		printType = "%g\n";
	}
	else if(sizeof(startingLink->value) == sizeof(char))
	{
		printType = "%c\n";
	}
	else
	{
		printType = "%s\n";
	}

	while(startingLink && itemsRemain > 0)
	{
		printf(printType, startingLink->value);
		startingLink = startingLink->next;
		itemsRemain -= 1;
	}
}

/* Reverse the deque

	param: 	q		pointer to the deque
	pre:	q is not null and q is not empty
	post: 	the deque is reversed
*/
void reverseCirListDeque(struct cirListDeque *q)
{
	printf("Reverse linked list\n");	
	assert(q != 0);
	assert(q->size > 0); 
	struct DLink *tempLink = malloc(sizeof(struct DLink));
	assert(tempLink != 0);	
	struct DLink *startingLink = malloc(sizeof(struct DLink));
	assert(startingLink != 0);
	startingLink = q->Sentinel->next;
	tempLink = 0;

	while(startingLink)
	{
//Swapping previous and next pointers	
		tempLink = startingLink->next;
		startingLink->next = startingLink->prev;
		startingLink->prev = tempLink;
//If the previous pointer is null, then we're reaching the front of the linked list. 
//Set front sentinel to the current link.
		if(startingLink->prev == 0)
		{
			q->Sentinel->next = startingLink;
		}
//If the next pointer is null, then we're reach the end of the linked list.
//Set back sentinel to the current link.
		if(startingLink->next == 0);
		{
			q->Sentinel->prev = startingLink;
		}
//Move to the next link (prev as everything has swapped)
		startingLink = startingLink->prev;
	}
	free(tempLink);
}
