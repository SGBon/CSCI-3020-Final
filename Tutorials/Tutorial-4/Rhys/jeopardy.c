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


}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players){


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
        printf("Enter the name of player %d. \n", i);
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
    char *category = (char*)calloc(256,sizeof(char));
    int value;
    // Perform an infinite loop getting command input from users until game ends
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {

    	for(int i = 0; i < 4; i++){
    		selectedPlayer = players[turnOrder[i] - 1];

    		printf("%s's turn. \n", selectedPlayer.name);
    		display_categories();
    		printf("\nSelect a category: \n");
    		scanf("%s", category);
    		printf("\nSelect a value: \n");
    		scanf("%d", &value);

    		display_question(category, value);


    	}
        // Call functions from the questions and players source files
	
        // Execute the game until all questions are answered
       
        // Display the final results and exit
    }



    free(turnOrder);
    return EXIT_SUCCESS;
}
