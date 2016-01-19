
#include <stdio.h>
#include <pthread.h>

void *hello_world(void *arg){
  usleep(rand()%5);
  fprintf(stdout,"Hello world!\n");
  return NULL;
}

void *goodbye(void *arg){
  usleep(rand()%5);
  fprintf(stdout,"Goodbye!\n");
  return NULL;
}

int main(int argc, char *argv[]){

  //thread objects
  pthread_t helloth;
  pthread_t byeth;

  // create threads
  pthread_create(&helloth, 0, hello_world,NULL);
  pthread_create(&byeth, 0, goodbye,NULL);

  // join threads
  pthread_join(helloth,0);
  pthread_join(byeth,0);

  return 0;
}
