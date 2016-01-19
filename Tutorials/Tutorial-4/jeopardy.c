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
#include <stdbool.h>
#include <time.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here

// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char **tokens){
    // Setup answer buffer and delim description
    const char delim[2] = " ";
    char answer[BUFFER_LEN] = {0};
    char *temp;
    
    // Tokens the first 3 words to get past the "who is" or "what is"
    temp = strtok(input, delim);   
    temp = strtok(NULL, delim);
    temp = strtok(NULL, delim);
    // Starts the loop to token the entire string, then pushes it into answer 
	while(temp != NULL){
		strcat(answer, temp);
		temp = strtok(NULL, delim);
		if(temp != NULL){
			strcat(answer, " ");
		}
	} 
	*tokens = answer;

}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players){
    printf("\n-----------------------\n"); //Can't actually test this until we integrate. Let me know if something fucks up - Rhys
    printf("Scores:\n");
    printf("-----------------------\n");
    player *rank;
    int loc;
    for(int i = 0; i < 4; i++){
        rank->score = -1;    // set the rank value to be lower than what it is possible to score
        for (int j = 0; j < 4; j++){
            if (rank->score < players[j].score){ // Compare the rank score to the player score and find the largest one.                        rank = players[j];      // record it's location...
                loc = j;
            }
        }
        printf("%d - %s : %d\n", i, rank[i].name, rank[i].score); // Print the player data
        players[loc].score = -2;    // Set the printed player's score to -2 (prevents them from being selected again)
    } // Loop until done 4 times.
    printf("-----------------------\n");
}

void generateTurnOrder(int *turns)
{
    for(int i = 0; i <= 3; i=i){
        int r = rand()%4;
        r++;
        for (int j = 0; j <= 3; j++){
        	if (turns[j] == r){
        		break;
        	}

        	if (j == 3){
        		turns[i] = r;
        		i++;
        	}
		}
    }

}

int main(int argc, char *argv[])
{
	srand(time(NULL));
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    player players[4];
 
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };

    // Display the game introduction and prompt for players namesc
    printf("Welcome to Jeopardy: CSCI-3020 Edition! \n \n");
    // initialize each of the players in the array
    for (int i = 0; i < 4; i++){ 
        printf("Enter the name of player %d. \n", i+1); // Adds all the players/Initializes them.
        scanf("%s", players[i].name);
        printf("\n");
        players[i].score = 0;
    }

    initialize_game();
    int *turnOrder = (int*)malloc(4*sizeof(int));
    generateTurnOrder(turnOrder);

    printf("Turn Order: \n");
    printf("--------------- \n");
    for (int i = 0; i <= 3; i++){
    	printf("%d \n", turnOrder[i]);
    }
    printf("---------------\n");
    
    player selectedPlayer;
    char *category = (char*)calloc(BUFFER_LEN,sizeof(char)); // Setting up all the variables.
    char answer[256]; 
    char **tok;

    int value, qCompleted;
    qCompleted = 0;

    // Perform an infinite loop getting command input from users until game ends
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {

    	for(int i = 0; i < 4; i++){
    		selectedPlayer = players[turnOrder[i] - 1]; //selects players in the order of the turnOrder array, not i = 0 to 4

    		printf("%s's turn. \n", selectedPlayer.name);
    		display_categories();
    		printf("\nSelect a category: \n");
    		scanf("%s", category);
    		printf("\nSelect a value: \n");
    		scanf("%d", &value);

            if (!already_answered(category, value)){
                display_question(category, value);
                scanf("%s", answer);
                printf("Test");
                //printf("%s", answer);
                tokenize(answer, tok); // TODO: I don't know what arguements you're going to need, Colton.

                if(valid_answer(category, value, tok)){
                    printf("Correct!\n");
                    selectedPlayer.score += value;  // Question class is going to need to mark the question as completed
                    qCompleted++;                   // When ever valid_answer is called.
                } else {
                    printf("Incorrect!\n");
                    qCompleted++;
                }

            } else {
                printf("\nQuestion has already been answered. Please select another one.\n");
                i--;
            }

    	}

        if(qCompleted >= 48){ //Can't actually test this until we integrate. Let me know if something fucks up - Rhys
            show_results(players);
            break;
        }
        // Call functions from the questions and players source files
	
        // Execute the game until all questions are answered
       
        // Display the final results and exit
    }


	free(questions);
    free(turnOrder);
    free(category);
    free(answer);
    free(tok);
    return EXIT_SUCCESS;
}
