#include <stdio.h>
#include <omp.h>
int main (int argc, char *argv[])
{
    int thread_num, nthreads = 16;
    //ifdef _OPENMP is stating that if openMP is installed and run as part of the compile parameter, then the line between the ifdef 
    //statement will run. If openMP is used, then this ifdef creates the threads needed for the bottom part of the code.
    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
    printf("Testing OpenMP, you should see each thread print...\n");
    //pragma omp parallel passes out the commands to be run equally between the available threads. The brackets are the parts that 
    //you want to be run in parallel.
    #pragma omp parallel
    {
        //pragma omp critical defines the critical section of the code. This means that only one thread can be in this part of 
        //the code at a time. 
        #pragma omp critical
        {
          //If openMP is installed, then the number of the thread is aquired from the thread, and printed to the screen.
          #ifdef _OPENMP
          thread_num = omp_get_thread_num();
          printf("This thread = %d\n", thread_num);
          #endif
        }
    }
    return 0;
}
