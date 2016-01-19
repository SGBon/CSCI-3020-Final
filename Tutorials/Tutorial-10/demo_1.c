/*
 * Tutorial 10 MPI Demo for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Author: Jonathan Gillett
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0

int main (int argc, char* argv[])
{
    int proc_id;            // Process rank
    int n_proc;             // Number of processes

    // Initialize MPI, takes in the arguements given to the program and sets the number of threads. This call is only called
    //by the main thread.
    MPI_Init(&argc, &argv);
    
    // Returns the process ID of the current thread calling the function.
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    
    // Get the current number of processes in the group.
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    // Display a "Hello world" message from each child/parent
    //If the process ID is the master, then...
    if (proc_id == MASTER)
    {
        printf( "Hello world from MATER, process %d of %d\n", proc_id, n_proc);
    }
    //If the process ID is a child, then...
    else
    {
        printf( "Hello world from SLAVE, process %d of %d\n", proc_id, n_proc);
    }

    // Required to terminate all MPI processes
    MPI_Finalize();
}
