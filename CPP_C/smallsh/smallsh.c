/*
 * Program name: smallsh.c
 * Author: Huy Pham
 * Written on: 11/14/2015
 * Description: This program is a small shell program. It will handle three basic commands
 * - status, exit and cd. The remaining status will be executed via exec() function.
 * Credits:
 * (1) Split string into array
 * http://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-in-an-array
 * (2) Change directory 
 * http://stackoverflow.com/questions/9493234/chdir-to-home-directory
 * (3) Signal handler using sigaction()
 * http://codingfreak.blogspot.com/2009/09/catching-and-ignoring-signals-sigaction.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LENGTH 2048
#define MAX_ARGS 512

// Struct to handle signal for main process and child process.
struct sigaction mProcess, cProcess;

/*
 * This function will check for the background process.
 */
void checkBackground()
{
	int wpid, status;
	while((wpid=waitpid(-1,&status,WNOHANG))>0)
	{
		printf("##Background pid %d is done.\n", wpid);
		if(WIFEXITED(status))
		{
			printf("##Exit Value %d.\n",WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))
		{
			printf("##Terminated by signal %d.\n",WTERMSIG(status));
		}
	}
//	return status;
}
/* 
 * This function will get the input from the command line and store it in an array.
 * The function will also return the number of arguments back to the calling function.
 */
int readInput(char input[], char *argArray[], int *inputNeeded, int *outputNeeded)
{
	//check background processes
	checkBackground();
	
	fflush(stdout);
//	printf("Enter your command\n");
	printf(": ");
    fgets(input, MAX_LENGTH, stdin);
	int argCount = 0;
	char _gt[] = ">";
	char _lt[] = "<";
	argArray[argCount] = strtok(input," \n");		//(1)
	while (argArray[argCount] != NULL)
	{	
		// strip out the < and > signs from the command line
		// and set the appropriate indicator instead.
		int ignore = 0;
		if(strcmp(argArray[argCount], _lt) == 0)
		{
			*inputNeeded = 1;
			ignore = 1;	
		} 
		else if(strcmp(argArray[argCount], _gt) == 0)
		{
			*outputNeeded = 1;
			ignore = 1;
		} 
		
		if (ignore == 0)
		{
			argCount++;
		}
		argArray[argCount] = strtok(NULL, " \n");	
	}
	return argCount;
}
/*
 * This function will create a process.
 */
int createProcess(char *argArray[], int argCount, int processState, int inputNeeded, int outputNeeded) 
{
//	printf ("pid:%d\n", (int) getpid());
	int status;
	
	int pid = fork();
	if (pid < 0) 					//Fork failed
	{
		fprintf(stderr,"fork failed\n");
		status = 1;	
	}
	else if (pid == 0)				//In child process
	{
		// set signal handler to default action when it occurs in the child process (3)
		cProcess.sa_handler = SIG_DFL;
		sigaction(SIGINT, &cProcess, NULL);
		// create pipe
		if(inputNeeded)
		{
			int fd0 = open(argArray[1], O_RDONLY);
			if(fd0 == -1)			//badfile
			{
				status = 1;
				return status;	
			}
			else
			{
//				printf("input needed - fd0: %i\n", fd0);
				dup2(fd0, STDIN_FILENO);
				close(fd0);
			}
		}		
		if(outputNeeded)
		{
			int fd1 = open(argArray[1], O_WRONLY|O_CREAT|O_TRUNC, 0664);
			if(fd1 == -1)
			{
				status = 1;
				return status;
			}
			else
			{
//				printf("output needed - fd1: %i\n", fd1);
				dup2(fd1, STDOUT_FILENO);
				close(fd1);
				argArray[1] = NULL;			//initialize the second argument to NULL as the name
			}								//has been used as the output file
		}
		execvp(argArray[0], argArray);
		perror(argArray[0]);
//		printf("Child process completed\n");
	}
	else							//In parent process 
	{
//		printf("In parent process - pid:%d - child-pid:%d\n", (int) getpid(), pid);
		if (processState == 1) 		//foreground process
		{
//			printf("Waiting until child process is completed!\n");
			waitpid(pid, &status, 0);
//			printf("Child process is completed\n");
		}
		else
		{
			printf("background pid is %d\n", pid);
		}
	}
	return status;
}
/*
 * This function will evaluate each command and route it to the appropriate function.
 * The function also returns a return code based on the status of each command.
 */
int evaluateCommand(char *argArray[], int argCount, int inputNeeded, int outputNeeded, int *lastStatus)
{
	int foreground = 1, exitShell = 0;
	char _status[] = "status";
	char _exit[] = "exit";
	char _cd[] = "cd";
	char _and[] = "&";
	char _comment = '#';
	
	if(argCount > 0)
	{
		// Determine foreground vs background process
		if (strcmp(argArray[argCount - 1], _and) == 0)
		{
			foreground = 0;						//set foreground to false
			argArray[argCount - 1] = NULL;		//remove the '&'
			argCount--;							//decrease the argCount			
		}
		// Get first character. This is used to determine if the line should be a comment line.
		char firstChar = argArray[0][0];

		if (strcmp(argArray[0], _status) == 0)
		{
			printf("Exit value: %d\n", *lastStatus);
		} // Exit logic
		else if (strcmp(argArray[0], _exit) == 0)
		{
			exitShell = 1;
		} // Change directory logic
		else if (strcmp(argArray[0], _cd) == 0)
		{
			if(argCount == 1)
			{
				chdir(getenv("HOME"));			//use the getenv() to read the enviroment variable					
			}									//for HOME and assign it to chdir (2)
			else
			{
				if(chdir(argArray[1]) == -1) 	//invalid path
				{
					printf("%s: invalid directory\n", argArray[1]);
					*lastStatus = 1;
				}
				else
				{
					*lastStatus = 0;
				}
			}
		} // Allow comment '#'
		else if (firstChar == _comment)
		{
//			printf("You have entered '#'\n");
		} // Other commands will go here
		else
		{
			argArray[argCount] = NULL;
			int cp = createProcess(argArray, argCount, foreground, inputNeeded, outputNeeded);
			if (cp == 0)
			{
				*lastStatus = 0;		//set last status
			}
			else
			{
				if(foreground == 1)		//set status only if this is a foreground process
				{
					*lastStatus = 1;
				}
			}
		}
	} // Allow blank line
	else
	{
//		printf("You have entered a blank line\n");
	}		
	// check back ground process to eliminate zombie processes
	checkBackground();
	return exitShell;
}

/*
 * This is the main logic of the program.
 */
int main (int argc, char *argv[])
{
	int exit = 0, inputNeeded, outputNeeded, lastStatus = 0;
	do
	{
		//set signal handler to ignore terminate signal in the main process (3)
		mProcess.sa_handler = SIG_IGN;
		sigaction(SIGINT, &mProcess, NULL);
		char input[MAX_LENGTH];
		char *argArray[MAX_ARGS];
		inputNeeded = 0;
	 	outputNeeded = 0;	
		int numArg = readInput(input, argArray, &inputNeeded, &outputNeeded);
		exit = evaluateCommand(argArray, numArg, inputNeeded, outputNeeded, &lastStatus);
	} while (exit == 0);

	return 0;
}
