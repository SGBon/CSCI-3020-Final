#define _XOPEN_SOURCE 600
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
	pid_t PID1 = 0;
	pid_t PID2 = 0;

	PID1 = fork();

    if (PID1 == 0) {
        sleep(1);
        FILE *fp;
        char buff[255];

    	fp = fopen("child1.txt", "r");
    	fscanf(fp, "%s", buff);
    	printf("%s", buff);
    	fscanf(fp, "%s", buff);
    	printf(" %s\n", buff);
    } else {
        PID2 = fork();
        if(PID2 == 0){
        	sleep(1);
        	FILE *fp;
        	char buff[255];

    		fp = fopen("child2.txt", "r");
    		fscanf(fp, "%s", buff);
    		printf("%s", buff);
    		fscanf(fp, "%s", buff);
    		printf(" %s\n", buff);
        } else {
        	FILE *fp;

  			fp = fopen("child1.txt", "w+");
    		fprintf(fp, "child 1\n");
    		fclose(fp);

    		FILE *fp2;

  			fp2 = fopen("child2.txt", "w+");
    		fprintf(fp2, "child 2\n");
    		fclose(fp2);
        }
    }
}