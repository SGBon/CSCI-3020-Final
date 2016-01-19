#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define MAX 1000000

int main (int argc, const char *argv[]){

  int nthreads = atoi(argv[1]);

  #ifdef _OPENMP
  omp_set_num_threads(nthreads);
  #endif

  int n = MAX;
  double y[MAX];
  double dx = 1/(double)MAX;
  double x;

  #pragma omp parallel for private(x)
    for(int i = 0;i<n;i++){
      x = i * dx;
      y[i] = exp(x) * cos(x) * sin(x) * sqrt(5 * x + 6.0);
    }

    return 0;
}
