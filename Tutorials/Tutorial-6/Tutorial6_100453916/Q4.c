#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUMBERS 10

typedef struct
{
  int buffer[BUFFER_SIZE];
  int in;
  int out;
  sem_t full;
  sem_t empty;
  sem_t sem;
} sbuf_t;

sbuf_t shared_data;
int user_nums[NUMBERS];


void *producer()
{
  int item;

  for (int i=0; i < NUMBERS; i++)
  {
    item = user_nums[i];

    sem_wait(&shared_data.empty);
    sem_wait(&shared_data.sem);
    shared_data.buffer[shared_data.in] = item;
    shared_data.in = (shared_data.in+1)%BUFFER_SIZE;
    printf("Producing: %d\n",item);

    sem_post(&shared_data.sem);
    sem_post(&shared_data.full);

    sleep(rand()%2);
  }
  return NULL;
}

void *consumer()
{
  int item;

  for (int i=NUMBERS; i > 0; i--) {
    sem_wait(&shared_data.full);
    sem_wait(&shared_data.sem);
    item=shared_data.buffer[shared_data.out];
    shared_data.buffer[shared_data.out]=0;
    shared_data.out = (shared_data.out+1)%BUFFER_SIZE;
    printf("Consuming: %d\n", item);

    sem_post(&shared_data.sem);
    sem_post(&shared_data.empty);

    sleep(rand()%2);
  }
  return NULL;
}

int main()
{
  time_t t;
  srand((unsigned)time(&t));

  for(int i=0;i<BUFFER_SIZE;i++){
    shared_data.buffer[i]=0;
  }
  for(int i=0;i<NUMBERS;i++){
    scanf("%d",&user_nums[i]);
  }

  sem_init(&shared_data.full, 0, 0);
  sem_init(&shared_data.empty, 0, BUFFER_SIZE);
  sem_init(&shared_data.sem, 0,1);

  pthread_t producer_thread,consumer_thread;

  pthread_create(&producer_thread, NULL, producer,NULL);
  pthread_create(&consumer_thread, NULL, consumer,NULL);

  pthread_join(producer_thread,NULL);
  pthread_join(consumer_thread,NULL);

  printf("Buffer: ");
  for(int i=0;i<BUFFER_SIZE;i++){
    printf("%d ",shared_data.buffer[i]);
  }
  printf("\n");
}
