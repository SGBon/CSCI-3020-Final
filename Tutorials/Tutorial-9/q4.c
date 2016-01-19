#include <stdio.h>
#include <omp.h>

#define matrix_size 100

int main (int argc, char *argv[])
{
  int nthreads = 4;

  #ifdef _OPENMP
  omp_set_num_threads(nthreads);
  #endif

  int a[matrix_size][matrix_size];
  int b[matrix_size][matrix_size];
  int c[matrix_size][matrix_size];

  int sum;

  //Initilize arrays
  for(int y=0;y<matrix_size;y++){
    #pragma omp parallel for
    for(int x=0;x<matrix_size;x++){
      a[y][x]=y;
      b[y][x]=x;
    }
  }

  //Perform matrix multiplication
  for(int y=0;y<matrix_size;y++){
    for(int x=0;x<matrix_size;x++){
      sum = 0;
      #pragma omp parallel for reduction(+:sum)
      for(int z=0;z<matrix_size;z++){
        sum += (a[y][z] * b[z][x]);
      }
      c[y][x] = sum;
    }
  }

  //Display for testing if matrix size is appropriate to display
  if(matrix_size<=30){
    for(int y=0;y<matrix_size;y++){
      for(int x=0;x<matrix_size;x++){
        printf("%5d ",c[y][x]);
      }
      printf("\n");
    }
    //else just show first 30
  }else{
    for(int y=0;y<30;y++){
      for(int x=0;x<30;x++){
        printf("%5d ",c[y][x]);
      }
      printf("\n");
    }
  }


  return 0;
}
