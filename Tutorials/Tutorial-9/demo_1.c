#include <stdio.h>
#include <omp.h>
int main (int argc, char *argv[])
{
    printf("This statement will always print.\n");
    //ifdef _OPENMP is stating that if openMP is installed and run as part of the compile parameter, then the line between the ifdef statement will run. This ifdef prints the second statement only if openMP is run.
    #ifdef _OPENMP
    printf("This statement will only print when compiled with \"-fopenmp\"\n");
    #endif
    return 0;
}
