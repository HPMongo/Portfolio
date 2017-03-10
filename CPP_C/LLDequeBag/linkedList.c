/* CS261 - Assignment3 - linkedListMain.c -
* Name: Huy Pham
* Date: 02/01/2015
* Solution description: This is a deque data structure implementation 
* using linked list.*/


#include "linkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/* Double Link*/
struct DLink {
	TYPE value;
	struct DLink * next;
	struct DLink * prev;
};

/* Double Linked List with Head and Tail Sentinels  */

struct linkedList{
	int size;
	struct DLink *firstLink;
	struct DLink *lastLink;
};

/*
	initList
	param lst the linkedList
	pre: lst is not null
	post: lst size is 0
*/

void _initList (struct linkedList *lst) {
	lst->firstLink = malloc(sizeof(struct DLink));
	assert(lst->firstLink != 0);
	lst->lastLink = malloc(sizeof(struct DLink));
	assert(lst->lastLink != 0);
	lst->firstLink->next = lst->lastLink;
	lst->lastLink->prev = lst->firstLink;
	lst->size = 0;
//	printf("In initList()\n");
}

/*
 createList
 param: none
 pre: none
 post: firstLink and lastLink reference sentinels
 */

struct linkedList *createLinkedList()
{
	struct linkedList *newList = malloc(sizeof(struct linkedList));
	_initList(newList);
	return(newList);
}

/*
	_addLinkBeforeBefore
	param: lst the linkedList
	param: l the  link to add before
	param: v the value to add
	pre: lst is not null
	pre: l is not null
	post: lst is not empty
*/

/* Adds Before the provided link, l */

void _addLinkBefore(struct linkedList *lst, struct DLink *l, TYPE v)
{
//	printf("In addLinkBefore() \n");
	assert(lst->firstLink != 0);
	assert(l != 0);
	struct DLink *newLink = malloc(sizeof(struct DLink));
	assert(newLink != 0);
	newLink->value = v;
	l->prev->next = newLink;
	newLink->prev = l->prev->next;
	l->prev = newLink;
	newLink->next = l;
	lst->size += 1;	
}


/*
	addFrontList
	param: lst the linkedList
	param: e the element to be added
	pre: lst is not null
	post: lst is not empty, increased size by 1
*/

void addFrontList(struct linkedList *lst, TYPE e)
{
//	printf("In addFrontList() \n");
	assert(lst->firstLink != 0);
	struct DLink *newLink = malloc(sizeof(struct DLink));
	assert(newLink != 0);
	newLink->value = e;
	lst->firstLink->next->prev = newLink;
	newLink->next = lst->firstLink->next;
	newLink->prev = lst->firstLink;
	lst->firstLink->next = newLink;
	lst->size += 1;
//	printf("Adding %d to %p size %d\n", e, newLink, lst->size);	
}

/*
	addBackList
	param: lst the linkedList
	param: e the element to be added
	pre: lst is not null
	post: lst is not empty, increased size by 1
*/

void addBackList(struct linkedList *lst, TYPE e) {
  
//	printf("In addBackList() \n");
	assert(lst->lastLink != 0);
	struct DLink *newLink = malloc(sizeof(struct DLink));
	assert(newLink != 0);
	newLink->value = e;
	lst->lastLink->prev->next = newLink;
	newLink->prev = lst->lastLink->prev;
	newLink->next = lst->lastLink;
	lst->lastLink->prev = newLink;
	lst->size += 1;	
}

/*
	frontList
	param: lst the linkedList
	pre: lst is not null
	pre: lst is not empty
	post: none
*/

TYPE frontList (struct linkedList *lst) {
	assert(lst->firstLink->next != 0);
	assert(lst->size != 0);
	return (lst->firstLink->next->value);
}

/*
	backList
	param: lst the linkedList
	pre: lst is not null
	pre: lst is not empty
	post: lst is not empty
*/

TYPE backList(struct linkedList *lst)
{
	assert(lst->lastLink->prev != 0);
	assert(lst->size != 0);
	return (lst->lastLink->prev->value);
}

/*
	_removeLink
	param: lst the linkedList
	param: l the linke to be removed
	pre: lst is not null
	pre: l is not null
	post: lst size is reduced by 1
*/

void _removeLink(struct linkedList *lst, struct DLink *l)
{
	assert(lst->firstLink->next != 0);
	assert(l != 0);
	l->prev->next = l->next;
	l->next->prev = l->prev;
	free(l);
	lst->size -= 1;
}

/*
	removeFrontList
	param: lst the linkedList
	pre:lst is not null
	pre: lst is not empty
	post: size is reduced by 1
*/

void removeFrontList(struct linkedList *lst) {
	assert(lst->firstLink->next != 0);
	assert(lst->size > 0);
	_removeLink(lst, lst->firstLink->next);
}

/*
	removeBackList
	param: lst the linkedList
	pre: lst is not null
	pre:lst is not empty
	post: size reduced by 1
*/

void removeBackList(struct linkedList *lst)
{	
	assert(lst->lastLink->prev != 0);
	assert(lst->size > 0);
	_removeLink(lst, lst->lastLink->prev);
}

/*
	isEmptyList
	param: lst the linkedList
	pre: lst is not null
	post: none
*/

int isEmptyList(struct linkedList *lst) {
	assert(lst->firstLink->next != 0);
	if(lst->size > 0)
		return(0);	//Not empty
	else
		return(1);	//Empty
}


/* Function to print list
 Pre: lst is not null
 */
void _printList(struct linkedList* lst)
{
	assert(lst->firstLink != 0);
	char *printType;
	int itemsRemain = 0;
	struct DLink *startingLink = malloc(sizeof(struct DLink));
	startingLink = lst->firstLink->next;
	itemsRemain = lst->size;
//Test for TYPE data
	if(sizeof(startingLink->value) == sizeof(int))
	{
		printType = "%d \n";
	}
 	else if(sizeof(startingLink->value) == sizeof(float))
	{
		printType = "%f \n";
	}
 	else if(sizeof(startingLink->value) == sizeof(char))
	{
		printType = "%c \n";
	}
 	else
	{
		printType = "%s \n";
	}
	
	while(startingLink && itemsRemain > 0)
	{
		printf(printType, startingLink->value);
		startingLink = startingLink->next;
		itemsRemain -= 1;
	}	
	free(startingLink);	
}

/* 
	Add an item to the bag
	param: 	lst		pointer to the bag
	param: 	v		value to be added
	pre:	lst is not null
	post:	a link storing val is added to the bag
 */
void addList(struct linkedList *lst, TYPE v)
{
	assert(lst->firstLink != 0);
	addFrontList(lst, v);
}

/*	Returns boolean (encoded as an int) demonstrating whether or not
	the specified value is in the collection
	true = 1
	false = 0

	param:	lst		pointer to the bag
	param:	e		the value to look for in the bag
	pre:	lst is not null
	pre:	lst is not empty
	post:	no changes to the bag
*/
int containsList (struct linkedList *lst, TYPE e) {
	if(isEmptyList(lst) == 0)	//The list is not empty
	{
		int count = 0;
		struct DLink *startingLink = malloc(sizeof(struct DLink));
		startingLink = lst->firstLink->next;
		while(startingLink)
		{
			if(startingLink->value == e)
			{
				count += 1;
				startingLink = 0;	//Found the item, get out of loop	
			}
			else
			{
				startingLink = startingLink->next;
			}
		}	
		free(startingLink);
		if(count > 0)
		{
			return (1);	//Item found
		}
		else
		{
			return (0); 	//Item not found
		}
	}
	else	
		return(0);		//The list is empty - item is not in the bag
}

/*	Removes the first occurrence of the specified value from the collection
	if it occurs

	param:	lst		pointer to the bag
	param:	e		the value to be removed from the bag
	pre:	lst is not null
	pre:	lst is not empty
	post:	e has been removed
	post:	size of the bag is reduced by 1
*/
void removeList (struct linkedList *lst, TYPE e) {
	assert(lst->firstLink->next != 0);
	assert(lst->size > 0);
	if(containsList(lst, e) == 1)
	{
		struct DLink *startingLink = malloc(sizeof(struct DLink));
		startingLink = lst->firstLink->next;
		while(startingLink)
		{
			if(startingLink->value == e)
			{
				_removeLink(lst,startingLink);
				startingLink = 0;	//Delete the item, get out of loop	
			}
			else
			{
				startingLink = startingLink->next;
			}
		}	
		free(startingLink);
	}
	else
	{
		printf("Item is not in the list!\n");
	}
}
