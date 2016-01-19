#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include <pthread.h>
#include <semaphore.h>

#define MAX 5
sem_t sem;
int total_sum = 0;

void *factorial(void *arg){
  int pre = (int)arg;
  int factorial = 1;

  for(int i = pre;i>0;i--){
    factorial = factorial*i;
    fprintf(stdout,"Factorial = %d\n",factorial);
  }

  // critical section
  sem_wait(&sem);

  total_sum += factorial;

  sem_post(&sem);

  fprintf(stdout,"Calculated factorial = %d\n",factorial);

  return NULL;
}

int main(int argc, char* argv[]){
  int numbers[MAX] = {0};

  pid_t PID;

  FILE *fl = fopen("numbers.txt","w");

  fprintf(stdout,"Please enter 5 numbers: ");
  for(int i = 0;i<MAX;i++){
    scanf("%d",&numbers[i]);
    fprintf(fl,"%d\n",numbers[i]);
  }

  fclose(fl);

  PID = fork();

  if(PID < 0){
    fprintf(stderr,"Fork failed\n");
    return -1;
  }else if(PID == 0){ //

    pthread_t fath[MAX];
    sem_init(&sem,0,1);
    FILE *fl = fopen("numbers.txt","r");
    int input[MAX] = {0};

    for(int i = 0;i<MAX;i++){
      fscanf(fl,"%d",&input[i]);
      pthread_create(&fath[i],0,factorial,(void *) input[i]);
    }

    fclose(fl);

    for(int i = 0;i<MAX;i++){
      pthread_join(fath[i],0);
    }

    fl = fopen("sum.txt","w");
    fprintf(fl,"%d\n",total_sum);
    fclose(fl);

    sem_destroy(&sem);
    exit(0);

  }else{ // parent process
    wait(NULL);
    FILE *fl = fopen("sum.txt","r");
    int sum = 0;
    fscanf(fl,"%d",&sum);
    fclose(fl);

    fprintf(stdout,"The sum from all the threads is: %d\n",sum);
  }

  return 0;
}
