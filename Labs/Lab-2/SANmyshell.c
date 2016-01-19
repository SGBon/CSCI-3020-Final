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

// Put global environment variables here
char CWD[BUFFER_LEN] = {0};
char PATH[BUFFER_LEN] = {0};

// Define functions declared in myshell.h here
void refresh(void){
	getcwd(CWD,sizeof(CWD));
	printf("%s> ",CWD);
}

void environ(void){
	fprintf(stdout,"Buffer Length --> %d\n",BUFFER_LEN);
	fprintf(stdout,"PATH --> %s\n",PATH);
}

void rCom(char *command, char *arg[], int argcount){

	// ----------- SANTIAGO REDIRECTION
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

	// ----------- END SANTIAGO

	if (strcmp(command, "cd") == 0){
		chdir(arg[0]);
	}

	// dir command -- display contents of directory
	// ls is an alias
	else if(strcmp(command,"dir") == 0 || strcmp(command,"ls") == 0){
		// if no argument, display contents of CWD instead
		printf("\n Start \n");
		if(argcount == 0){
			arg[0] = CWD;
			printf("\nIt Works!\n");
		}
		dir(arg[0]);
	}

	//Environment variables
	else if(strcmp(command,"environ") == 0){
		printf("\n%s\n", command);
		environ();
	}

	//Clear Screen
	else if(strcmp(command,"clr") == 0){
		printf("\n%s\n", command);
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

	// Unsupported command
	else{
		fputs("Unsupported command, use help to display the manual\n\n", stderr);
	}

	// --------- SANTIAGO START
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

	// --------- END SANTIAGO

}

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
	//pid_t pid;

	char buffer[BUFFER_LEN] = { 0 };
	char command[BUFFER_LEN] = { 0 };
	//char arg[BUFFER_LEN] = { 0 };

	char *args[MAX_ARG];
	int argcount = 0;

	refresh();
	strncpy(PATH,CWD,BUFFER_LEN);
	strcat(PATH,"myshell.exe");


	if(argc > 1){

		FILE *fl;
		fl = fopen (argv[1], "r");

		while(fgets(buffer, BUFFER_LEN, fl) != NULL){

			memset(&command[0], 0, sizeof(command));
			memset(&args[0], 0, sizeof(args));

			argcount = multitokenize(buffer,command,args);

			if(strcmp(command, "quit") == 0){
				return EXIT_SUCCESS;
			} else {
				rCom(command,args, argcount);
			}

			refresh();

		}

		rCom("halt", args, 0);
		return EXIT_SUCCESS;
	}

	while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
	{

		memset(&command[0], 0, sizeof(command));
		memset(&args[0], 0, sizeof(args));

		argcount = multitokenize(buffer,command,args);

		if(strcmp(command, "quit") == 0){
			return EXIT_SUCCESS;
		} else {
			rCom(command,args, argcount);
		}

		refresh();
	}
	return EXIT_SUCCESS;
}
