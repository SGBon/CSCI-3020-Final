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
#include "SANutility.h"


// Define your utility functions here, these will most likely be functions that you call
// in your myshell.c source file

void clr(void){
  for(int i =0;i<30;i++){
    printf("\n");
  }
}

void dir(char *directory){
          struct dirent *dp;

          DIR *dirp;

          // otherwise display contents of specified directory
          if((dirp = opendir(directory)) == NULL){
            printf("ls: cannot open %s\n",directory);
          }else{
            fprintf(stdout,"Displaying contents of %s\n",directory);
            while((dp = readdir(dirp)) != NULL){
              // skip displaying the . and ..
              if(strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") == 0){
                continue;
              }
              // display the files and directories
              printf("%s\n",dp->d_name);
            }
            printf("\n");
            closedir(dirp);
          }
}

void echo(char *comment){
	fprintf(stdout,"%s ",comment);
}

void help(){

}

void halt(){
	printf("Press enter to continue . . . ");
	getchar(); // Sadd
	printf("\n\n");
}
