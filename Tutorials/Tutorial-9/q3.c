#include <stdio.h>
#include "omp.h"

int main (int argc, char *argv[])
{
	//int i = 0;
	int *x = malloc(100000000*sizeof(int));    
	long norm = 0;

	for (int i = 0; i < 100000000; i++){
		x[i] = rand() % 100;
	}

	printf("\nSerial execution:\n");

	double begin;
	double end;
	double diff;

	begin = omp_get_wtime();
	for (int i = 0; i < 100000000; i++){
		norm += fabs(x[i]);
	}
	end = omp_get_wtime();

	printf("Norm: %d\n", norm);
	printf("Time: %f\n", (end - begin));

	diff = end - begin;

	norm = 0;

	printf("\nParallel execution:\n");

	begin = omp_get_wtime();
	#pragma omp parallel for reduction (+:norm)
	for (int i = 0; i < 100000000; i++){
		norm += fabs(x[i]);
	}
	end = omp_get_wtime();

	diff = (end - begin) - diff;

	printf("Norm: %d\n", norm);
	printf("Time: %f\n\n", (end - begin));

	printf("Difference: %f\n\n", diff);

    return 0;
}
