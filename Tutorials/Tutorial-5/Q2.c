
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *bellCurve(void *arg)
{
    float curved = *((float *)arg) * 1.5;

    printf("%f\n",curved);

    return NULL;
}

/**
 * Basic example that showing threading interleaving.
 */
int main(int argc, char *argv[]) {

    // Thread object
    pthread_t pth[5];
    float* grades[5];

    printf("Please input 5 student grades. \n");

    for(int i = 0; i < 5; i++){
        grades[i] = (float*)malloc(sizeof(float));
        scanf("%f", grades[i]);
    }

    printf("Curved Grades:");


    // Create the threads
    for(int i = 0; i < 5; i++){
        pthread_create(&pth[i], 0, bellCurve, grades[i]);
    }

    // wait for our threads to finish before continuing
    for(int i = 0; i < 5; i++){
        pthread_join(pth[i], 0);
    }

    for(int i = 0;i<5;i++){
      free(grades[i]);
    }

    return 0;
}
