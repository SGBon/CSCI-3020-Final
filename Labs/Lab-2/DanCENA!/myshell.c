	/*
	 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
	 *
	 * Copyright (C) 2015, <GROUP MEMBERS>
	 * All rights reserved.
	 *
	 */
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/types.h>
	#include <string.h>
	#include "utility.h"
	#include "myshell.h"

	// Put macros or constants here using #define
	#define BUFFER_LEN 256

	// Put global environment variables here
	char CWD[BUFFER_LEN] = {0};

	// Define functions declared in myshell.h here
	void refresh(void){
	  getcwd(CWD,sizeof(CWD));
	  printf("%s> ",CWD);
	}

	void environ(void){
		//Seems redundant to post working directory again
	  //fprintf(stdout,"Current working directory --> %s\n",CWD);
	  fprintf(stdout,"Buffer Length --> %d\n",BUFFER_LEN);
	}

	void tokenize(char *str,char *cmd,char *arg){
	   char s[3] = " >";
	   char *token;

	   // Get command
	   token = strtok(str, s);
	   //Cut out newline garbage if string command exists
	   if(token!=NULL){
		   size_t ln = strlen(token)-1;
			if(token[ln]=='\n')
				token[ln] = '\0';
			strcpy(cmd,token);
	   }else if(str!=NULL){
		strcpy(cmd,str);
	   }

	   // Get argument
	   token = strtok(NULL, s);
	   while(token!=NULL){
	   //Cut out newline garbage if argument exists
			size_t ln = strlen(token)-1;
			if(token[ln]=='\n'){
				token[ln] = '\0';
				strcpy(arg,token);
			}
			token=strtok(NULL,s);
		}
	}

	int main(int argc, char *argv[])
	{
		// Input buffer and and commands
		char buffer[BUFFER_LEN] = { 0 };
		char command[BUFFER_LEN] = { 0 };
		char arg[BUFFER_LEN] = { 0 };

		// Parse the commands provided using argc and argv

		// Assign the CWD
		refresh();

		// Perform an infinite loop getting command input from users
		while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
		{
			//Clear buffers before getting new commands
			memset(&command[0], 0, sizeof(command));
			memset(&arg[0], 0, sizeof(arg));

			// Perform string tokenization to get the command and argument
			tokenize(buffer,command,arg);

			// Check the command and execute the operations for each command

			// cd command -- change the current directory
		  if (strcmp(command, "cd") == 0){
			chdir(arg);
		  }

		  // dir command -- display contents of directory
		  // ls is an alias
		  else if(strcmp(command,"dir") == 0 || strcmp(command,"ls") == 0){
			// if no argument, display contents of CWD instead
			if(strcmp(arg,"") == 0){
			  strncpy(arg,CWD,BUFFER_LEN);
			}
			dir(arg);
		  }

		  //Environment variables
		  else if(strcmp(command,"environ") == 0){
			environ();
		  }

		  //Clear Screen
		  else if(strcmp(command,"clr") == 0){
			clr();
		  }

			//Echo
			else if(strcmp(command,"echo") == 0){
				echo(arg);
			}

			//Pause Shell
			else if(strcmp(command,"pause")==0){
				halt();
			}

			// quit command -- exit the shell
			else if (strcmp(command, "quit") == 0){
				return EXIT_SUCCESS;
			}

			// Unsupported command
			else{
				fputs("Unsupported command, use help to display the manual\n\n", stderr);
			}

			refresh(); // at end of loop, always display CWD
		}
		return EXIT_SUCCESS;
	}
