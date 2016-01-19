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
	#include "DANutility.h"
	#include "DANmyshell.h"

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
//----------Rhys Change----------//
	void rCom(char *command, char *arg){
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
			else if(strcmp(command,"halt")==0){
				halt();
			}

			// Unsupported command
			else{
				fputs("Unsupported command, use help to display the manual\n\n", stderr);
			}
			
	}
//----------Change End-----------//

	void tokenize(char *str,char *cmd,char *arg){
	   const char s[2] = " ";
	   char *token;


	   token = strtok(str, s);

	   if(token!=NULL){
		   size_t ln = strlen(token)-1;
			if(token[ln]=='\n')
				token[ln] = '\0';
			strcpy(cmd,token);
	   }else if(str!=NULL){
		strcpy(cmd,str);
	   }


	   token = strtok(NULL, s);

	   if(token!=NULL){
			size_t ln = strlen(token)-1;
			if(token[ln]=='\n')
				token[ln] = '\0';
			strcpy(arg,token);
	   }
	}

	int main(int argc, char *argv[])
	{
		char buffer[BUFFER_LEN] = { 0 };
		char command[BUFFER_LEN] = { 0 };
		char arg[BUFFER_LEN] = { 0 };

		refresh();
		//----------Rhys Change----------//
		if(argc > 1){
			
			printf("%d",argc);
			printf(argv[1]);
			
			FILE *fl;
			fl = fopen (argv[1], "r");

			while(fgets(buffer, BUFFER_LEN, fl) != NULL){

			memset(&command[0], 0, sizeof(command));
			memset(&arg[0], 0, sizeof(arg));
				

			tokenize(buffer,command,arg);
			//----------Rhys Change----------//
			if(strcmp(command, "quit") == 0){
				return EXIT_SUCCESS;
			} else {
				rCom(command,arg);
			}
			//----------Change End-----------//
			refresh(); 
			}

			rCom("halt", "");
			return EXIT_SUCCESS;
		}
		//----------Change End-----------//
			
		while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
		{

			memset(&command[0], 0, sizeof(command));
			memset(&arg[0], 0, sizeof(arg));
			
			tokenize(buffer,command,arg);

//----------Rhys Change----------//
			if(strcmp(command, "quit") == 0){
				return EXIT_SUCCESS;
			} else {
				rCom(command,arg);
			}
//----------Change End-----------//

			refresh(); 
		}
		return EXIT_SUCCESS;
	}
