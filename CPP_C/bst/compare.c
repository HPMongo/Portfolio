/* CS261 - Assignment 4 - compare.c
 * Name: Huy Pham
 * Date: 02/15/2015
 * Solution description: helper function to do the comparison between the predefined TYPE
 * and print out the output of TYPE data. This is needed in order for the BST to work 
 * correctly.
 */

#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include "structs.h"

/*----------------------------------------------------------------------------
 very similar to the compareTo method in java or the strcmp function in c. it
 returns an integer to tell you if the left value is greater then, less then, or
 equal to the right value. you are comparing the number variable, letter is not
 used in the comparison.

 if left < right return -1
 if left > right return 1
 if left = right return 0
 */

 /*Define this function, type casting the value of void * to the desired type.
  The current definition of TYPE in bst.h is void*, which means that left and
  right are void pointers. To compare left and right, you should first cast
  left and right to the corresponding pointer type (struct data *), and then
  compare the values pointed by the casted pointers.

  DO NOT compare the addresses pointed by left and right, i.e. "if (left < right)",
  which is really wrong.
 */
int compare(TYPE left, TYPE right)
{
	assert(left != 0);
	assert(right != 0);
	struct data *tempLeft = (struct data*) left;
	struct data *tempRight = (struct data*) right;
	if(tempLeft->number < tempRight->number)
	{
		return -1;
	}
	else if(tempLeft->number == tempRight->number)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*Define this function, type casting the value of void * to the desired type*/
void print_type(TYPE curval)
{
	assert(curval != 0);
	struct data *tempType = (struct data*) curval;
	printf("Number:  %d Name: %s\n", tempType->number, tempType->name);
}


