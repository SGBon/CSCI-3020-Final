/*
 * Tutorial 10 MPI Demo for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Author: Jonathan Gillett
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
 #include <unistd.h>

// Define process 0 as MASTER
#define MASTER 0
#define ARRSIZE 100

void master(int n_proc)
{
    long a[ARRSIZE][ARRSIZE] = { 0 };
    long b[ARRSIZE][ARRSIZE] = { 0 };
    long chunk[ARRSIZE] = { 0 };
    long rchunk[ARRSIZE] = { 0 };
    long c[ARRSIZE][ARRSIZE] = { 0 };
    int n_sent = 0, n_recv = 0; 
    MPI_Status status;

    printf("\nGood luck reading this.\n\n");

    sleep(3);

    printf("Starting Matrices: \n");
    printf("a: \n");

    for (int i = 0; i < ARRSIZE; i++) {
        for (int j = 0; j < ARRSIZE; j++) {
            a[i][j] = i+j;
            b[i][j] = i+j;
            printf("%ld ", a[i][j]);
        }
        printf("\n");
    }

    printf("\n ------ \n\n");

    printf("b: \n");

    for (int i = 0; i < ARRSIZE; i++) {
        for (int j = 0; j < ARRSIZE; j++) {
            printf("%ld ", b[i][j]);
        }
        printf("\n");
    }

    printf("\n ------ \n\n");

    printf("Multiplied Matrix: \n");

    MPI_Bcast(&b[0][0], ARRSIZE*ARRSIZE, MPI_LONG, 0, MPI_COMM_WORLD);

    for (int i = 1; i < n_proc; i++) {
        for (int j = 0; j < n_proc; j++){
            chunk[j] = a[i-1][j];
        }
        MPI_Send(&chunk, ARRSIZE, MPI_LONG, i, n_sent, MPI_COMM_WORLD);
        n_sent++;
    }

    for (int i = 1; i < n_proc; i++) {
        MPI_Recv(rchunk, ARRSIZE, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        for (int j = 0; j < ARRSIZE; j++){
            c[status.MPI_TAG][j] = rchunk[j];
        }
    }

    printf("\n");
    for (int i = 0; i < ARRSIZE; i++){
        for (int j = 0; j < ARRSIZE; j++){
            printf("%ld ", c[i][j]);
        }
        printf("\n");
    } 

    printf("\nDone!\n");

}


void slave(int proc_id)
{
    long b[ARRSIZE][ARRSIZE] = {0};
    long chunk[ARRSIZE]= { 0 };
    long rchunk[ARRSIZE] = { 0 };
    int n_recv = 0;          
    MPI_Status status;   

    MPI_Bcast(b, ARRSIZE*ARRSIZE, MPI_LONG, 0, MPI_COMM_WORLD);

    MPI_Recv(chunk, ARRSIZE, MPI_LONG, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    n_recv = status.MPI_TAG;

    for (int i = 0; i < ARRSIZE; i++){
        for (int j = 0; j < ARRSIZE; j++){
            rchunk[i] = rchunk[i] + (b[j][i] * chunk[j]);
        }
    }


    MPI_Send(rchunk, ARRSIZE, MPI_LONG, MASTER, n_recv, MPI_COMM_WORLD);

}


int main (int argc, char* argv[])
{
    int proc_id;            // Process rank
    int n_proc;             // Number of processes

    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the current number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    // Get the current process id
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    

    if (proc_id == MASTER)
    {
        master(n_proc);
    }
    else
    {
        slave(proc_id);
    }

    // Required to terminate all MPI processes
    MPI_Finalize();
}