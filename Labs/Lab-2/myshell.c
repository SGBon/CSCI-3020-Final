/*
* MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
*
* Copyright (C) 2015, <Rhys, Colton, Santiago, Dan>
* All rights reserved.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define MAX_ARG 10

// Global Environment variables
char CWD[BUFFER_LEN] = {0};
char PATH[BUFFER_LEN] = {0};

// Display current working directory
void refresh(void){
	getcwd(CWD,sizeof(CWD));
	printf("%s> ",CWD);
}

//Display environment variables
void environ(void){
	fprintf(stdout,"Buffer Length --> %d\n",BUFFER_LEN);
	fprintf(stdout,"PATH --> %s\n",PATH);
}

//Read in commands
void rCom(char *command, char *arg[], int argcount){

	// define redirections variables
	char *rein = "<";
	char *reout = ">";
	char *reapp = ">>";
	int stdin_dupl;
	int stdout_dupl;
	int temp_in;
	int temp_out;
	int nuargc = argcount;

	// get stdout and stdin
	stdin_dupl = dup(0);
	stdout_dupl = dup(1);

	for(int i = 0;i<argcount;i++){
		// compare the argument to check for redirects
		// input redirector
		if(strcmp(arg[i],rein)==0){
			nuargc = i;
			temp_in = open(arg[i+1],O_RDONLY);
			// replace stdin with the file descriptor
			dup2(temp_in,STDIN_FILENO);
		// output redirectors for write truncate and write append together
		}else if(strcmp(arg[i],reout)==0){
			if (nuargc == argcount){
				nuargc = i;
			}
			temp_out = open(arg[i+1],O_WRONLY|O_CREAT|O_TRUNC);
			// replace
			dup2(temp_out,STDOUT_FILENO);
		}else if(strcmp(arg[i],reapp)==0){
			if (nuargc == argcount){
				nuargc = i;
			}
			temp_out = open(arg[i+1],O_WRONLY|O_CREAT|O_APPEND);
			// replace
			dup2(temp_out,STDOUT_FILENO);
		}
	}

	for(int i = nuargc;i<argcount;i++){
		memset(&arg[i], 0, sizeof(arg[i]));
	}

	argcount = nuargc;

	if (strcmp(command, "cd") == 0){
		chdir(arg[0]);
	}

	// dir command -- display contents of directory
	// ls is an alias
	else if(strcmp(command,"dir") == 0 || strcmp(command,"ls") == 0){
		// if no argument, display contents of CWD instead;
		if(argcount == 0){
			arg[0] = CWD;
		}
		dir(arg[0]);
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
		echo(arg[0]);
	}

	//Pause Shell
	else if(strcmp(command,"halt")==0){
		halt();
	}


	//Displays the readme file to the terminal
	else if(strcmp(command,"help") == 0){
		//Displays the entire readme file to the terminal with no arg count
		if(argcount == 0){
			FILE *fp;
			char buff[1024];
			int count = 0;
			//Open file to be read
			fp = fopen("readme", "r");
			//Prints the entire file to the terminal
			while (fgets (buff, 1024, fp)) {
				printf("%s",buff);
				count++;
				//Displays 30 lines of code and then pauses the terminal until enter is pressed
				if(count == 30){
					printf("Press any key to continue displaying manual.");
					getchar();
					count = 0;
				}
			}
			fclose (fp);
			//If there is an argument, search the readme and display that command
		} else {
			FILE *fp;
			char buff[1024];
			//Open the file for reading
			fp = fopen("readme","r");
			while (fgets (buff, 1024, fp)) {
				//Set up the command we are looking for
				char check[1000] = "Name - ";
				strcat(check,arg[0]);
				strcat(check,"\n");
				//Go through the entire file and check to see if the line matches the beginning of the command description
				//When it finds the right command, start printing the file to the screen
				if(strcmp(buff,check) == 0){
					_Bool flag = 0;
					printf("%s",buff);
					//While we are still in that description of the command, print out the lines
					while(flag == 0){
						fgets (buff, 1024, fp);
						printf("%s",buff);
						//Prints out until =END= is found, which is the end of the command description
						if(strcmp(buff,"=END=\n") == 0){
							flag = 1;
						}
					}
				}
			}
			fclose(fp);
		}
	}

	// Unsupported command
	else{
		fputs("Unsupported command, use help to display the manual\n\n", stderr);
	}

	// flush buffer so it writes to file, then close files
	fflush(stdout);
	close(temp_in);
	close(temp_out);

	// restore streams
	dup2(stdin_dupl,0);
	dup2(stdout_dupl,1);

	// close old streams
	close(stdin_dupl);
	close(stdout_dupl);
}

//Take an input string and tokenize it into command and arguments
int multitokenize(char *str,char *cmd,char *args[]){
	const char s[2] = " ";
	char *token;
	int argc = 0;


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

	while(token!=NULL){
		size_t ln = strlen(token)-1;
		if(token[ln]=='\n')
		token[ln] = '\0';
		args[argc] = token;
		token = strtok(NULL,s);
		argc++;
	}
	return argc;
}

int main(int argc, char *argv[])
{
	//Input buffer to get user input
	char buffer[BUFFER_LEN] = { 0 };
	//Current command to be used
	char command[BUFFER_LEN] = { 0 };
	//Current arguments to use
	char *args[MAX_ARG];
	//argument count
	int argcount = 0;

	refresh();
	strncpy(PATH,CWD,BUFFER_LEN);
	strcat(PATH,"myshell.exe");

	if(argc > 1){ // If myshell is started with arguements given to the main
		// (If a file of commands is passed to it)
		FILE *fl;
		fl = fopen (argv[1], "r"); // Read from file

		while(fgets(buffer, BUFFER_LEN, fl) != NULL){ // For the entire length of the file...

			memset(&command[0], 0, sizeof(command));
			memset(&args[0], 0, sizeof(args));

			argcount = multitokenize(buffer,command,args); // Tokenize the command from the line...

			if(strcmp(command, "quit") == 0){
				return EXIT_SUCCESS;
			} else {
				rCom(command,args, argcount); // Take the tokenized command and run it.
			}

			refresh();

		}

		rCom("halt", args, 0); // Once the file is finished, exit.
		return EXIT_SUCCESS;
	}

	while (fgets(buffer, BUFFER_LEN, stdin) != NULL) // if no file is given, execute from the command line
	{

		memset(&command[0], 0, sizeof(command));
		memset(&args[0], 0, sizeof(args));

		argcount = multitokenize(buffer,command,args); // tokenize the command from the command line

		if(strcmp(command, "quit") == 0){ // If the command is 'quit', quit.
		return EXIT_SUCCESS;
	} else {
		rCom(command,args, argcount);
	}

	refresh();
}
return EXIT_SUCCESS;
}
