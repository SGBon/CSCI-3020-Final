#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <sys/resource.h>

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define ON_BLACK   "\033[40m"
#define NORMAL     "\033[0m"

int main(int argc, char *argv[])
{
     
  printf("--Program Start--\n");

  pid_t pid = fork();
  printf("--Process: %d--\n", getpid());

  if (pid != 0){
    sleep(5);
    kill(pid,SIGINT);
    printf(RED ON_BLACK "Hi, I'm process:%d, and I've killed your beloved other process. >:D\n", getpid());
  } else {
    for (int i = 0; i < 20; i++){
      printf(YELLOW ON_BLACK "Hi! I'm process:%d, and I'm currently running! :D\n", getpid());
      sleep(1);
    }
  }

  printf(NORMAL);
  exit(0);
}