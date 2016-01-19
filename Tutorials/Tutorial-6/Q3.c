#define _XOPEN_SOURCE 600
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define FIVE 5

int* moving_sum[5];
pthread_barrier_t barr;
sem_t sem;

struct numInd
{
   int num;
   int index;
} numInd;  

void *Factorial(void *arg)
{
    struct numInd str;
    str = *(struct numInd*)arg;
    
    int fact = 1;
    for(int i = str.num; i > 0; i--){
        if(str.num == 0){
            break;
        }
        fact = fact * i;
    }

    //printf("Thread %d factorial = %d\n", str.index, fact);

    int rc = pthread_barrier_wait(&barr);

    if (str.index > 0){

        while (1==1) {
            sem_wait(&sem);
           // printf("Thread %d is Waiting. Prev value: %d, Curr value:%d\n", str.index, *moving_sum[str.index - 1], fact);
            if (*moving_sum[str.index - 1] > 0){
                //printf("Thread %d is executing. Prev value: %d, Curr value:%d\n", str.index, *moving_sum[str.index - 1], fact);
                *moving_sum[str.index] = fact + *moving_sum[str.index - 1];
                //printf("Thread %d has executed. Curr value:%d\n", str.index, *moving_sum[str.index]);
                sem_post(&sem);
                break;
            }
            sem_post(&sem);
        }


    } else {

        *moving_sum[str.index] = fact;

    }

    return 0;
}


int main (int argc, char *argv[]){

    pthread_t *pth[FIVE];
    struct numInd test[FIVE];

    pthread_barrier_init(&barr,NULL,5);

    sem_init(&sem, 0, 1);

    for(int i = 0; i < 5; i++){
        moving_sum[i] = (int*)malloc(sizeof(int));
        *moving_sum[i] = 0; 
    }

    printf("Please input 5 numbers.\n");

    for(int i = 0; i < 5; i++){
        scanf("%d", &test[i].num);
        test[i].index = i;
    }
    
    for(int i = 0;i<5;i++){
        pthread_create(&pth[i],NULL,Factorial,(void *) &test[i]);
    }


    for(int i = 0;i<FIVE;i++){
        pthread_join(*pth[i],0);
    }

    printf("---moving_sum contents---\n");
    for (int i = 0; i < 5; i++) {
        printf("index %d = %d\n", i, *moving_sum[i]);
    }

    // Clean up
    sem_destroy(&sem);
    pthread_barrier_destroy(&barr);

    for(int i = 0;i<5;i++){
      free(moving_sum[i]);
    }

    return 0;
}


/*
int main(int argc, char *argv[]) {

    pthread_t *pth[FIVE];

    for(int i = 0; i < 5; i++){
        moving_sum[i] = (int*)malloc(sizeof(int));
        *moving_sum[i] = 0; 
    }

    printf("Please input 5 numbers.\n");

    for(int i = 0; i < 5; i++){
        scanf("%d", moving_sum[i]);
    }

    // Initialize the semaphore.
    sem_init(&sem, 0, 1);

    // Create the threads

    for(int i = 0; i < 5; i++){

        struct numInd *test;
        (*test).num = *moving_sum[i];
        (*test).index = i;

        printf("Test->num : %d\n", (*test).num);
        printf("Test->index : %d\n", (*test).index);

        printf("Thread Create : %d\n", i);
        pthread_create(pth[i], NULL, Factorial, (void *) i);
        printf("Thread Created : %d\n", i);
    }

    printf("Done.\n");

    // Clean up
    sem_destroy(&sem);

    return 0;
}
*/
/*
int* moving_sum[5];
sem_t sem;

struct numInd
{
   int num;
   int index;
} numInd;  


void *Factorial(void *arg)//struct numInd data)
    {
        printf("phase1\n");
        int fact = 1;
        for(int i = numInd.num; i > 0; i--){
            if(numInd.num == 0){
                break;
            }
            fact = fact * i;
        }
        printf("phase2\n");
        *moving_sum[numInd.index] = fact;
        printf("phase3\n");


        for(int i = numInd.index; i > 0; i--){
            sem_wait(&sem);
            *moving_sum[numInd.index] = *moving_sum[numInd.index] + *moving_sum[numInd.index-1];
            sem_post(&sem);
        }
        printf("phase4\n");
        printf("Thread %d:\n",numInd.index);
        printf("%d\n", *moving_sum[numInd.index]);

        return 0;
    }

int main(int argc, char *argv[]) {
    pthread_t pth[5];

    for(int i = 0; i < 5; i++){
        moving_sum[i] = (int*)malloc(sizeof(int));
        *moving_sum[i] = 0; 
    }

    printf("Please input 5 numbers.\n");

    for(int i = 0; i < 5; i++){
        scanf("%d", moving_sum[i]);
    }

    sem_init(&sem, 0, 1);

    printf("Threads\n");
    for(int i = 0; i < 5; i++){
        struct numInd* test;
        test->num = *moving_sum[i];
        test->index = i;

        printf("phase1\n");
        printf("Thread Create %d:\n", i);
        pthread_t pth;
        pthread_create(&pth[i], 0, Factorial, (void *) test);
        printf("phase1\n");
    }

    for(int i = 0; i < 5; i++){
        pthread_join(pth[i], 0);
    }

    for(int i = 0;i<5;i++){
      free(moving_sum[i]);
    }

    return 0;
}*/