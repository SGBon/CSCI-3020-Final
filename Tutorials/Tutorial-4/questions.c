/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "questions.h"

/*
typedef struct {
    char category[MAX_LEN];
    char question[MAX_LEN];
    char answer[MAX_LEN];
    int value;
    bool answered;
} question;
*/

// Initializes the array of questions for the game
void initialize_game(void)
{
	int category;
	char question[MAX_LEN];
	char answer[MAX_LEN];
	int value;

	int qnum=0;
	qs=0;

	//Open file and ready for reading
	FILE *f;
	f = fopen("questions.txt","r");
	char line[MAX_LEN];

	//Read in ammount of questions and setup array
	fgets(line,MAX_LEN,f);
	sscanf(line,"%d",&qs);
	questions = calloc(MAX_LEN,qs*sizeof(question));

	//Read questions
	while(qnum<qs){
		fgets(line,MAX_LEN,f);
		sscanf(line,"%d",&category);
		fgets(line,MAX_LEN,f);
		sscanf(line,"%[^\t\n]",&question);
		fgets(line,MAX_LEN,f);
		sscanf(line,"%[^\t\n]",&answer);
		fgets(line,MAX_LEN,f);
		sscanf(line,"%d",&value);

		strcpy(questions[qnum].category,categories[category]);
		strcpy(questions[qnum].question,question);
		strcpy(questions[qnum].answer,answer);
		questions[qnum].value = value;
		questions[qnum].answered = false;
		qnum++;
	}
	fclose(f);

	/* Confirm all Questions were loaded in properly
	for(int i=0;i<qs;i++){
		printf("%s\n",questions[i].category);
		printf("%s\n",questions[i].question);
		printf("%s\n",questions[i].answer);
		printf("%d\n",questions[i].value);
	}
	*/

}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
	for(int j=0;j<3;j++){
		printf("\n%s: ",categories[j]);
		for(int i=0;i<qs;i++){
			if(strcmp(questions[i].category,categories[j])==0){
				if(!questions[i].answered){
					printf("%d ",questions[i].value);
				}else{
					printf("X");
				}
			}
		}
	}
	printf("\n");
    // print categories and dollar values for each unanswered question in questions array
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{
	for(int i=0;i<qs;i++){
		if(strcmp(questions[i].category,category)==0){
			if(questions[i].value==value){
				printf("\n%s\n",questions[i].question);
			}
		}
	}
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
	printf("%s",category);
	printf("%d",value);
	printf("%s",answer);

	for(int i=0;i<qs;i++){
		if(strcmp(questions[i].category,category)==0){
			printf("%s",questions[i].category);
			if(questions[i].value==value){
				printf("%d",questions[i].value);
				if(strcmp(questions[i].answer,answer)==0){
					printf("%s",questions[i].answer);
					return true;
				}else{
					printf("%s",questions[i].answer);
					return false;
				}
			}
		}
	}
	return false;
    // Look into string comparison functions
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
	for(int i=0;i<qs;i++){
			if(strcmp(questions[i].category,category)==0){
				if(questions[i].value==value){
					return questions[i].answered;
			}
		}
	}
	return false;
    // lookup the question and see if it's already been marked as answered
}
