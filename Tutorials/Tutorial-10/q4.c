#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define process  0 as MASTER
#define MASTER 0

#define RANGE 1000 // numbers to look through

_Bool is_prime(int n){
  int limit = ceil(sqrt((double)n));

  for(int i = 2;i <= limit; i++){
    if((n % i) == 0){
      return 0;
    }
  }
  return 1;
}

void master(int n_proc){
  int primes[RANGE] = {0}; // our array to store primes
  int index = 0; // the next spot to insert into the primes array

  int space[2] = {0}; // array to send to slave processes
  int subLength = RANGE/(n_proc - 1) ; // length of arrays to send
  int subPrime[subLength]; // array we receive from slave processes

  int n_sent = 0; // amount of data sent
  MPI_Status status; // MPI status struct

  // initalize variable length array
  for(int i = 0;i<subLength;i++){
    subPrime[i] = 0;
  }

  // create range arrays and set start/stop
  // send arrays to slave processes
  for(int i = 1;i<n_proc;i++){
    space[0] = (i-1)*subLength + 1;
    space[1] = (i*subLength);

    // send off ranges to each process
    if(i < n_proc){
      MPI_Send(&space,2,MPI_INT,i,n_sent,MPI_COMM_WORLD);
      n_sent++;
    }
  }

  // receive the primes from slave processes
  for(int i = 0;i<n_proc-1;i++){
    // receive the subprimes
    MPI_Recv(subPrime,subLength,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);

    // copy the primes from the slave into the master primes array
    for(int i = 0;i<subLength;i++){
      primes[index] = subPrime[i];
      index++;
    }
  }

  // print out the primes
  printf("The prime numbers between 1 and %d are:\n",RANGE);
  for (int i = 0;i<RANGE;i++){
    if(primes[i]>0){
      printf("%d ",primes[i]);
    }
  }
  printf("\n");

}

void slave(int proc_id){
  int space[2] = {0}; // range to receive from master
  int subLength; // length of prime array
  int index = 0; // our prime counter
  int n_recv = 0; // data chunk received
  MPI_Status status; // status struct

  // get the range from MASTER
  MPI_Recv(space,2,MPI_INT,MASTER,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
  n_recv = status.MPI_TAG;

  subLength = space[1] - space[0];
  int subPrime[subLength]; // array to hold primes to send

  // initialize variable length array
  for(int i = 0;i<subLength;i++){
    subPrime[i] = 0;
  }

  // find primes
  for(int i = space[0]; i < space[1]; i++){
    if(is_prime(i)){
      subPrime[index] = i;
      index++;
    }
  }

  // send back the results
  MPI_Send(&subPrime,subLength,MPI_INT,MASTER,n_recv,MPI_COMM_WORLD);

}

int main(int argc, char* argv[]){
  int proc_id;
  int n_proc;

  // Initialize MPI
  MPI_Init(&argc,&argv);

  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD,&n_proc);

  // Get the current process id
  MPI_Comm_rank(MPI_COMM_WORLD,&proc_id);

  if (proc_id == MASTER){
    master(n_proc);
  }else{
    slave(proc_id);
  }

  // end program stuff
  MPI_Finalize();
}
