/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <Dan,Santiago,Rhys,Colton>
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

//Clear screen
void clr(void){
  for(int i =0;i<30;i++){
    printf("\n");
  }
}

//Display contents of current directory
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

//Print text statement
void echo(char *comment){
	printf("%s\n\n",comment);
}

//Pause program
void halt(){
	printf("Press any key to continue . . . ");
	getchar();
	printf("\n\n");
}
