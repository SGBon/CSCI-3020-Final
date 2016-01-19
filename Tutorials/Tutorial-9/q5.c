#include <stdio.h>
#include <omp.h>
#include <math.h>

int main (int argc, char *argv[])
{
  int nthreads = argc;

  long double n = 100000000;
  long double dx = 1/(n+1);
  long double x = 0;
  FILE *fp;
  fp = fopen("calculations.txt","w+");

  #ifdef _OPENMP
  omp_set_num_threads(nthreads);
  #endif

  #pragma omp parallel for private(x)
  for(int i = 0; i < (int) n; i++){
    x = i*dx;
    long double y = exp(x) * cos(x) * sin(x) * sqrt(5*x+6.0);
    #pragma omp critical
    {
      if((i % 1000000) == 0 && i != 0){
        fprintf(fp,"i = %d\nx = %Lf\ny = %Lf\n",i,x,y);
      } 
    }
  }  
  fclose(fp);
  return 0;
}