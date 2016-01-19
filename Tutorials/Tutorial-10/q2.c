#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

main(int argc, char** argv) {
  int R; //My process rank
  int P; //The number of processes
  int ping = 1;
  int pong = 2;
  int tag = 0;
  int  n = 100;   // number of ping-pong's back and forth
  const int size = 10000000; // length of message
  double *k;
  k  = (double *) malloc(size * sizeof(double));
  int i;
  MPI_Status status;
  MPI_Comm World = MPI_COMM_WORLD;
  double start_time, elapsed_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(World, &P);
  if (P < 3) { printf("this requires 3 processes\n"); return 0; }
  MPI_Comm_rank(MPI_COMM_WORLD, &R);

  if (R == ping) {
    start_time = MPI_Wtime();
    for (i = 0; i < n; ++i){
      k[0] = i;
      MPI_Send(k, size, MPI_DOUBLE, pong, tag, World);
      MPI_Recv(k, size, MPI_DOUBLE, pong, tag, World, &status);
    }
    elapsed_time = MPI_Wtime() - start_time;
    printf("%d ping-pongs in %g seconds,", n, elapsed_time);
    printf(" for an average send time of %g\n", elapsed_time/n;
  }

  if (R == pong)
  for (i = 0; i < n; ++i)
  {
    MPI_Recv(k, size, MPI_DOUBLE, ping, tag, World, &status);
    k[0]  += 1;
    MPI_Send(k, size, MPI_DOUBLE, ping, tag, World);
  }
  free(k);
  MPI_Finalize();
}
