/* CS261 - Assignment 2 - calc.c - Reverse Polish Calculator. 
 * Name: Huy Pham
 * Date: 01/25/2015
 * Solution description: This program will implement the basic calculator functions using
 * Reverse Polish Notation. The program will take and validate the inputs from the user
 * then perform the calculation and display the result.*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dynamicArray.h"

//Constants 
//Error codes:
int const TOOFEW = 101;
int const TOOMANY = 102;
int const NEGNUM = 103;

/* param: s the string
   param: num a pointer to double
   returns: true (1) if s is a number else 0 or false.
   postcondition: if it is a number, num will hold
   the value of the number
*/
int isNumber(char *s, double *num)
{
	char *end;
	double returnNum;

	if(strcmp(s, "0") == 0)
	{
		*num = 0;
		return 1;
	}
	else 
	{
		returnNum = strtod(s, &end);
		/* If there's anythin in end, it's bad */
		if((returnNum != 0.0) && (strcmp(end, "") == 0))
		{
			*num = returnNum;
			return 1;
		}
	}
	return 0;  //if got here, it was not a number
}

/* 	param: stack the stack being manipulated
 *	pre: n/a 
 *	post:result
 *	validate the total numbers in the stack
 *	the function will return:
 *		2 - if there are 2 or more numbers
 *		1 - if there is one number
 *		0 - if the stack is empty
 */
int checkSize(struct DynArr *stack)
{
	int result = 0;
	result = sizeDynArr(stack);
	if(result >= 2)
	{
		return 2;
	}
	else if(result == 1)
	{
		return 1; 
	}
	else
	{
		return 0;
	}
}

/*	param: error code, number of errors
 *	pre: n/a
 *	post: updated number of errors
 *	display error message based on the error code and update the count for total errors
 */
void displayError(int const errorCode, int *errorCount)
{
	if(errorCode == TOOFEW)
	{
		printf("There are too few numbers to perform the calculation. ");
		printf("Program is terminated!\n");
	}
	
	if(errorCode == TOOMANY)
	{
		printf("There are two many numbers in the input. Calculation is now terminated!\n");
	}
	
	if(errorCode == NEGNUM)
	{
		printf("Can't perform the operation on a negative number. Calculation is terminated!\n");
	}
	*errorCount +=1;
}
/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their sum is pushed back onto the stack.
*/
void add (struct DynArr *stack)
{
	double num1 = 0.0, num2 = 0.0;
	//Remove the top two elements from the stack, add them together and push the result back
	//to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);

	num2 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In add, num2 is: %f num1 is: %f \n", num2, num1);
	pushDynArr(stack,num2 + num1);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their difference is pushed back onto the stack.
*/
void subtract(struct DynArr *stack)
{
	double num1 = 0.0, num2 = 0.0;
	//Remove the top two elements from the stack, substract one from another and push the result back
	//to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);

	num2 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In sub,  num2 is: %f num1 is: %f \n", num2, num1);
	pushDynArr(stack,num2 - num1);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their quotient is pushed back onto the stack.
*/
void divide(struct DynArr *stack)
{
	double num1 = 0.0, num2 = 0.0;
	//Remove the top two elements from the stack, divide one from another and push the result back
	//to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);

	num2 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In div,  num2 is: %f num1 is: %f \n", num2, num1);
	pushDynArr(stack,num2 / num1);

}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their product is pushed back onto the stack.
*/
void multiply(struct DynArr *stack)
{
	double num1 = 0.0, num2 = 0.0;
	//Remove the top two elements from the stack, multiply one from another and push the result back
	//to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);

	num2 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In mult,  num2 is: %f num1 is: %f \n", num2, num1);
	pushDynArr(stack,num2 * num1);

}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their power is pushed back onto the stack.
*/
void power(struct DynArr *stack)
{
	double num1 = 0.0, num2 = 0.0;
	//Remove the top two elements from the stack, perform power operation and push the result back
	//to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);

	num2 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In power,  num2 is: %f num1 is: %f \n", num2, num1);
	pushDynArr(stack,pow(num2,num1));

}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the square is pushed back onto the stack.
*/
void square(struct DynArr *stack)
{
	double num1 = 0.0;
	//Remove the top element from the stack, square the number and push the result back
	//to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In square,  num1 is: %f \n", num1);
	pushDynArr(stack,num1 * num1);

}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the cube is pushed back onto the stack.
*/
void cube(struct DynArr *stack)
{
	double num1 = 0.0;
	//Remove the top element from the stack, cube the number and push the result back
	//to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In cube,  num1 is: %f \n", num1);
	pushDynArr(stack,num1 * num1 * num1);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the absolute value is pushed back onto the stack.
*/
void absolute(struct DynArr *stack)
{
	double num1 = 0.0;
	//Remove the top element from the stack, multiply the number by -1 if it's negative or leave
	//it as-is if it is positive and push the result back to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In abs,  num1 is: %f \n", num1);
	if(num1 < 0)
	{
		pushDynArr(stack,num1 * -1.0);
	}
	else
	{
		pushDynArr(stack,num1);
	}
}

/*	param: stack the stack being manipulated, errorCount - total error in the calculate function
	pre: the stack contains at least one element
	post: the top element is popped and 
	the square root is taken and pushed back onto the stack.
*/
void sqroot(struct DynArr *stack, int *errorCount)
{
	double num1 = 0.0;
	//Remove the top element from the stack, the square root is taken
	//and push the result back to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In sqrt,  num1 is: %f \n", num1);
	if(num1 < 0)
	{
		displayError(NEGNUM, errorCount);
	}
	else
	{
		pushDynArr(stack,sqrt(num1));
	}
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the exponential is calculated and pushed back onto the stack.
*/
void expo(struct DynArr *stack)
{
	double num1 = 0.0;
	//Remove the top element from the stack, calculate the exponential
	//and push the result back to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In expo,  num1 is: %f \n", num1);
	pushDynArr(stack,exp(num1));
}


/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the natural log is calculated and pushed back onto the stack.
*/
void nLog(struct DynArr *stack)
{
	double num1 = 0.0;
	//Remove the top element from the stack, calculate the natural log
	//and push the result back to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In nLog,  num1 is: %f \n", num1);
	pushDynArr(stack,log(num1));
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the base10 log is calculated and pushed back onto the stack.
*/
void base10Log(struct DynArr *stack)
{
	double num1 = 0.0;
	//Remove the top element from the stack, calculate the base10 log
	//and push the result back to the stack	
	num1 = topDynArr(stack);
	popDynArr(stack);
	
	//printf("In base10Log,  num1 is: %f \n", num1);
	pushDynArr(stack,log10(num1));
}

double calculate(int numInputTokens, char **inputString)
{
	int i, validationError = 0;
	double result = 0.0, tempNum = 0.0;
	char *s;
	struct DynArr *stack;

	//set up the stack
	stack = createDynArr(20);

	// start at 1 to skip the name of the calculator calc
	for(i=1;i < numInputTokens;i++) 
	{
		s = inputString[i];

		// Hint: General algorithm:
		// (1) Check if the string s is in the list of operators.
		//   (1a) If it is, perform corresponding operations.
		//   (1b) Otherwise, check if s is a number.
		//     (1b - I) If s is not a number, produce an error.
		//     (1b - II) If s is a number, push it onto the stack

		if(strcmp(s, "+") == 0)
		{
			if(checkSize(stack) == 2)
			{
				add(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s,"-") == 0)
		{
			if(checkSize(stack) == 2)
			{
				subtract(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "/") == 0)
		{
			if(checkSize(stack) == 2)
			{
				divide(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "x") == 0)
		{
			if(checkSize(stack) == 2)
			{
				multiply(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "^") == 0)
		{
			if(checkSize(stack) == 2)
			{
				power(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "^2") == 0)
		{
			if(checkSize(stack) == 2 || checkSize(stack) == 1)
			{
				square(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "^3") == 0)
		{
			if(checkSize(stack) == 2 || checkSize(stack) == 1)
			{
				cube(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "abs") == 0)
		{
			if(checkSize(stack) == 2 || checkSize(stack) == 1)
			{
				absolute(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "sqrt") == 0)
		{
			if(checkSize(stack) == 2 || checkSize(stack) == 1)
			{
				sqroot(stack, &validationError);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "exp") == 0)
		{
			if(checkSize(stack) == 2 || checkSize(stack) == 1)
			{
				expo(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "ln") == 0)
		{
			if(checkSize(stack) == 2 || checkSize(stack) == 1)
			{
				nLog(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else if(strcmp(s, "log") == 0)
		{
			if(checkSize(stack) == 2 || checkSize(stack) == 1)
			{
				base10Log(stack);
			}
			else
			{
				displayError(TOOFEW, &validationError);
			}
		}
		else 
		{
			/*When s is not an operator, add it to the stack*/
			if(strcmp(s, "pi") == 0)	//When s is pi
			{
				tempNum = 3.14159265;
				pushDynArr(stack, tempNum);
			}
			else if(strcmp(s, "e") == 0)	//When s is e
			{
				tempNum = 2.7182818;
				pushDynArr(stack, tempNum);
			}
			else if(isNumber(s,&tempNum))	//When s is a number
			{
				pushDynArr(stack, tempNum);
			} 
			else				//s is invalid, terminate the calculation
			{
				printf("%s is an invalid input. Calculation is now terminated!\n", s);
				i = numInputTokens;
				validationError += 1;
			}
		}
	}	//end for 

	 /* 
 	  * If there is no error, display the final result.
	  * Otherwise, return default (0.0) to the calling program.
	  */
	if(validationError == 0)
	{
		if(checkSize(stack) == 1)	//Only one number, result, left in the stack
		{
			result = topDynArr(stack);
			printf("Final result is: %f \n", result);
		}
		else				//More than one number left in the stack
		{
			displayError(TOOMANY, &validationError);
		}
 	}	
	deleteDynArr(stack);
	return result;
}

int main(int argc , char** argv)
{
	// assume each argument is contained in the argv array
	// argc-1 determines the number of operands + operators
	if (argc == 1)
		return 0;

	calculate(argc,argv);
	return 0;
}
