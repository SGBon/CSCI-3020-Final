#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>

#define NUM_STUD 10

pthread_barrier_t barr;

int total_grade=0;
int total_bellcurve=0;
int grades[NUM_STUD];
int curved[NUM_STUD];
pthread_mutex_t mutex;

void *read_grades(void *arg){

  FILE *fl;
  fl = fopen("grades.txt","r");

  for(int i = 0;i<NUM_STUD;i++){
    fscanf(fl,"%d",&grades[i]);
  }

  fclose(fl);

  // sync point
  int rc = pthread_barrier_wait(&barr);
  if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD){
        printf("Could not wait on barrier\n");
        exit(-1);
    }

  return NULL;
}

void *save_bellcurve(void *arg){

  int i = (int)arg;
  pthread_mutex_lock(&mutex);
  total_grade+=grades[i];
  curved[i] = grades[i]*1.5;
  total_bellcurve+=curved[i];
  pthread_mutex_unlock(&mutex);


  pthread_mutex_lock(&mutex);

  FILE *fl;
  fl = fopen("bellcurve.txt","a");
  fprintf(fl,"%d\n",curved[i]);
  fclose(fl);
  pthread_mutex_unlock(&mutex);

  //wait for all threads to finish
  int rc = pthread_barrier_wait(&barr);
  if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD){
        printf("Could not wait on barrier\n");
        exit(-1);
    }

  return NULL;
}

int main (int argc, char *argv[]){

  // declare threads
  pthread_t inputh;
  pthread_t *saveth[NUM_STUD];
  pthread_mutex_init(&mutex,NULL);

  // Barrier initialization

  if(pthread_barrier_init(&barr,NULL,1)){
    fprintf(stderr,"Could not create a barrier\n");
    return -1;
  }

  //read in the grades
  pthread_create(&inputh,NULL,read_grades,NULL);

  pthread_join(inputh,0);

  pthread_barrier_destroy(&barr);

  // reinitialze Barrier for bellcurve
  if(pthread_barrier_init(&barr,NULL,NUM_STUD)){
    fprintf(stderr,"Could not create a barrier\n");
    return -1;
  }

  // end of reading, begin bellcurve
  for(int i = 0;i<NUM_STUD;i++){
    pthread_create(&saveth[i],NULL,save_bellcurve,(void *) i);
  }

  for(int i = 0;i<NUM_STUD;i++){
    pthread_join(*saveth[i],0);
  }

  pthread_barrier_destroy(&barr);

  fprintf(stdout,"The total grade is: %d\n",total_grade);
  fprintf(stdout,"The average before the bellcurve is: %d\n",total_grade/NUM_STUD);
  fprintf(stdout,"The average after the bellcurve is: %d\n",total_bellcurve/NUM_STUD);



  return 0;
}
