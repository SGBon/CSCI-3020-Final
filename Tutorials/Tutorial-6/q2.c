#define _XOPEN_SOURCE 600
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

  pid_t PID = 0;

  // create the child
  PID = fork();

  if (PID < 0){
    fprintf(stderr,"Fork failed!\n");
    return -1;
  }else if(PID == 0){
    usleep(1000);
    fprintf(stdout,"Child process\n");
    return 0;
  }else{
    int status = 0;
    wait(&status);
    fprintf(stdout,"Parent process \nChild process returned with a value of %d\n",status);
  }

  return 0;
}
