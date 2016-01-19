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
	#include <sys/wait.h>
	#include <string.h>
	#include "SANutility.h"
	#include "SANmyshell.h"

	// Put macros or constants here using #define
	#define BUFFER_LEN 256
	#define MAX_ARG 10

	// Put global environment variables here
	char CWD[BUFFER_LEN] = {0};
	char PATH[BUFFER_LEN] = {0}; // Sadd

	// Define functions declared in myshell.h here
	void refresh(void){
	  getcwd(CWD,sizeof(CWD));
	  printf("%s> ",CWD);
	}

	void environ(void){
	  fprintf(stdout,"Buffer Length --> %d\n",BUFFER_LEN);
		fprintf(stdout,"PATH --> %s\n",PATH); // Sadd
	}

	// old tokenizer
	void tokenize(char *str,char *cmd,char *arg){
	   const char s[2] = " ";
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
	   //Cut out newline garbage if argument exists
	   if(token!=NULL){
			size_t ln = strlen(token)-1;
			if(token[ln]=='\n')
				token[ln] = '\0';
			strcpy(arg,token);
	   }
	}

	// ---------- SANTIAGO MULTI ARGUMENT TOKENIZATION ------
	// tokenize with MULTIPLE arguments
	int multitokenize(char *str,char *cmd,char *args[]){
		 const char s[2] = " ";
		 char *token;
		 int argc = 0; // arg counter

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
		 //Cut out newline garbage if argument exists
		 while(token!=NULL){  //if there's more arguments, keep going
			size_t ln = strlen(token)-1;
			if(token[ln]=='\n')
				token[ln] = '\0';
			//strcpy(args[argc],token);
			args[argc] = token; // char pointer lets this happen don't ask why
			token = strtok(NULL,s);
			argc++;
		 }
		 return argc;
	}

	// ----------- END SANTIAGO ---------------

	int main(int argc, char *argv[])
	{

		pid_t pid;

		// Input buffer and and commands
		char buffer[BUFFER_LEN] = { 0 };
		char command[BUFFER_LEN] = { 0 };
		char arg[BUFFER_LEN] = { 0 }; // to be removed - Santiago

		char *args[MAX_ARG]; // SANTIAGO ARGUMENT VECTOR
		int argcount = 0;

		// Parse the commands provided using argc and argv

		// Assign the CWD and shell path
		refresh();
		strncpy(PATH,CWD,BUFFER_LEN); // Sadd
		strcat(PATH,"myshell.exe"); // Sadd

		// Perform an infinite loop getting command input from users
		while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
		{
			//Clear buffers before getting new commands
			memset(&command[0], 0, sizeof(command));
			for(int i = 0;i<MAX_ARG;i++){
				memset(&args[i], 0, sizeof(args[i]));
			}

			// Perform string tokenization to get the command and argument
			//tokenize(buffer,command,arg);
			argcount = multitokenize(buffer,command,args);

			// Check the command and execute the operations for each command
			// cd command -- change the current directory
		  if (strcmp(command, "cd") == 0)
		  {
				chdir(args[0]);
		  }

		  // dir command -- display contents of directory
		  // ls is an alias
		  else if(strcmp(command,"dir") == 0 || strcmp(command,"ls") == 0){
			// if no argument, display contents of CWD instead
				if(argcount == 0){ // santiago, more changes here
					args[0] = CWD;
				}
				dir(args[0]);
			}

			//Environment variables
		  else if(strcmp(command,"environ") == 0){
			environ();
		  }

			// Clear Screen
		  else if(strcmp(command,"clr") == 0){
			clr();
		  }

			//Echo
			else if(strcmp(command,"echo") == 0){
				for(int i = 0;i<argcount;i++){
					echo(args[i]);
				}
				fprintf(stdout,"\n");
			}

			//Pause Shell
			else if(strcmp(command,"halt")==0){
				halt();
			}

			// other commands here...

			// quit command -- exit the shell
			else if (strcmp(command, "quit") == 0)
			{
				return EXIT_SUCCESS;
			}

			// execute external commands/files
			else
			{
				// ---- Santiago process forking ------
				// fork and execute "other" commands"
				_Bool bground;
				char *amper = "&";
				// check if program should run the background
				if(strcmp(args[argcount - 1],amper)){
					bground = 1;
					// since we don't want the & count as an argument
					memset(&args[argcount - 1], 0, sizeof(args[argcount - 1]));
				}else{
					bground = 0;
				}

				pid = fork();

				if(pid == 0) // child
				{
					if(execlp(command,command,*args,(char*) NULL) < 0) // execute command with arg
					{
						fprintf(stderr,"Command or File not found, type help to see a list of commands\n");
					}
					exit(1);
				}
				else if (pid < 0) // fork did not go through
				{
					fprintf(stderr,"Failed to fork");
					exit(1);
				}
				else // parent
				{
					if(!bground)
					{
						wait(NULL);
					}
				}
			}
				// ------- end Santiago ---------

			refresh(); // at end of loop, always display CWD
		}
		return EXIT_SUCCESS;
	}
