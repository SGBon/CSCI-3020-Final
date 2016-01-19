#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

struct student{
	char *name;
	int student_id;
	int grade;
};


void *bellcurve(void *s){
	struct student *stud = (struct student*) s;

	int temp = (*stud).grade;
	(*stud).grade = temp*1.5;
	printf("Student's Name: %s\n", (*stud).name);
	printf("Student's ID: %d\n", (*stud).student_id);
	printf("Student's Grade: %d\n", (*stud).grade);
}

int main(void){
	int i = 0;
	int grades[5];
	char *name[5];
	int student_id[5];

	struct student s[5];

	for(int j = 0; j < 5; j++){
		s[j].name = (char*)calloc(100,sizeof(char));
		s[j].grade = 0;
		s[j].student_id = 0;
	}

	while(i < 5){
		name[i] = (char *)calloc(50, sizeof(char));
		printf("Student #%d: Student's Name:\n",i+1);
		scanf("%s", name[i]);
		printf("Student #%d: Student's Student ID:\n",i+1);
		scanf("%d", &student_id[i]);
		printf("Student #%d: Student's Grade:\n",i+1);
		scanf("%d", &grades[i]);
		strcpy(s[i].name, name[i]);
		s[i].grade = grades[i];
		s[i].student_id = student_id[i];
		i++;
	}
	
	pthread_t pth1;
	pthread_t pth2;
	pthread_t pth3;
	pthread_t pth4;
	pthread_t pth5;

    pthread_create(&pth1, NULL, bellcurve, (void *)&s[0]);
    pthread_create(&pth2, NULL, bellcurve, (void *)&s[1]);
    pthread_create(&pth3, NULL, bellcurve, (void *)&s[2]);;
    pthread_create(&pth4, NULL, bellcurve, (void *)&s[3]);
    pthread_create(&pth5, NULL, bellcurve, (void *)&s[4]);

    pthread_join(pth1, 0);
    pthread_join(pth2, 0);
    pthread_join(pth3, 0);
    pthread_join(pth4, 0);
    pthread_join(pth5, 0);	  
}