#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int total_grade=0;
pthread_mutex_t mutex;

void *class_total(void *arg){

    int grade=(int)arg;
    pthread_mutex_lock(&mutex);
    total_grade+=grade;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {

    pthread_t* pth[10];
    pthread_mutex_init(&mutex,NULL);

    int grade;

    for(int i=0;i<10;i++){
        printf("Enter Student %d Grade: ",i+1);
        scanf("%d",&grade);
        pthread_create(&pth[i],NULL,class_total,(void*)grade);
    }

    for(int j=0;j<10;j++){
        pthread_join(pth[j],0);
    }

    printf("Total Grade Is %d\n",total_grade);

    pthread_mutex_destroy(&mutex);

    return 0;
}