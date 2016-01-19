/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Colton Howe, Rhys Agombar, Santiago Bonada, Dan Hope
 * All rights reserved.
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "banker.h"

// Put any other macros or constants here using #define
// May be any values >= 0
#define NUM_CUSTOMERS 100
#define NUM_RESOURCES 3

//Initalize the mutex lock to keep things valid
pthread_mutex_t mutex;

// Put global environment variables here
// Available amount of each resource
int available[NUM_RESOURCES];
int total[NUM_RESOURCES];


// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];

// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];

//Number of resources in use
int inUse[NUM_RESOURCES];

//Inital needs of each customer
int initialNeed[NUM_CUSTOMERS][NUM_RESOURCES];

bool check_safety(int n_customer, int request[NUM_CUSTOMERS][NUM_RESOURCES])
{
	//
	int work[NUM_RESOURCES];
	for (int i = 0; i < NUM_RESOURCES; i++){
		work[i] = inUse[i] + request[n_customer][i];
	}
	bool state = true;
	for (int i = 0; i < NUM_RESOURCES; i++){
			if(work[i] > total[i]){
				state = false;
			}
	}
	//Return the safety of the state. True is a safe state, false is a non-safe state.
	return state;
	
}

// Define functions declared in banker.h here
bool request_res(int n_customer, int request[])
{
	bool result = false, flag = true;

	//If the request is resonable, set the flag to true
	for (int i = 0; i < NUM_RESOURCES; i++){
		if (request[i] > need[n_customer][i]){
			flag = false;
			return result;
		}
		if (request[i] > available[i]){
			flag = false;
			return result;
		}
	}

	//If request is resonable, do the allocation for that request.
	if (flag == true){
		result = true;
		for (int i = 0; i < NUM_RESOURCES; i++){
			available[i] = available[i] - request[i]; 
			allocation[n_customer][i] = allocation[n_customer][i] + request[i];
			need[n_customer][i] = need[n_customer][i] - request[i];
		}
		result = true;
		printf("Customer %d request granted: %d %d %d - Available: %d %d %d - Need: %d %d %d\n", n_customer, request[0], request[1], request[2], available[0] ,available[1] ,available[2], need[n_customer][0], need[n_customer][1], need[n_customer][2]);
	}

	return result;

}

// Release resources, returns true if successful
bool release_res(int n_customer)
{
	printf("Customer %d resources released. Releasing %d %d %d\n",n_customer, initialNeed[n_customer][0], initialNeed[n_customer][1], initialNeed[n_customer][2]);
	for (int i = 0; i < NUM_RESOURCES; i++){
		available[i] = available[i] + allocation[n_customer][i];
		inUse[i] -= initialNeed[n_customer][i];
		allocation[n_customer][i] = 0;
 	}
 	return true;
 }

void procCust(int n_customer)
{
	int request[NUM_RESOURCES] = { 0 };
	srand(time(NULL));
	//For an infinite amount of time, request random numbers of resources. Once the resources are allocated to the need of the
	//process, release them.
	for (int j = 0; j < NUM_RESOURCES; j++){ // Generate the initial needs
		int val = rand() % (total[j] + 1);
		need[n_customer][j] = val;
		allocation[n_customer][j] = 0;
		maximum[n_customer][j] = val;
	}
	printf("Customer %d Inital Request: %d %d %d\n", n_customer, need[n_customer][0], need[n_customer][1], need[n_customer][2]);
	//Keeps track if this is the first run for that customer or not
	bool firstRun = true;
	//Sets the initial need for that customer
	for(int i = 0; i < NUM_RESOURCES; i++){
		initialNeed[n_customer][i] = need[n_customer][i];
	}
	while(true){
		bool exit = false;
		//Request random resources until it fills the need, then exit.
		while (true){
			for (int j = 0; j < NUM_RESOURCES; j++){ // Start generating the requests.
				request[j] = rand() % (total[j] + 1); // Can never request more than the max amount of resources we have
			}
			if (!(request[0] == 0 && request[1] == 0 && request[2] == 0)){
				break;
			}
		} 
		//If this is the first run, check if the request will leave the system in a safe state. If it is not safe, it will
		//decline the request. If it is not the first run, then its already been checked to be safe and will just run
		//the request.
		if(firstRun){
			pthread_mutex_lock (&mutex);
			bool safe = check_safety(n_customer, need);
			if(safe){
				printf("Customer %d: State is safe.\n", n_customer);
				bool result = request_res(n_customer, request);
				for(int i = 0; i < NUM_RESOURCES; i++){
					inUse[i] += initialNeed[n_customer][i];
				}
				firstRun = false;
			} else {
				printf("Customer %d: State is unsafe.\n", n_customer);
				printf("Customer %d request denied\n",n_customer);
				exit = true;
			}
			pthread_mutex_unlock (&mutex);
		} else {
			pthread_mutex_lock (&mutex);
			bool result = request_res(n_customer, request);
			pthread_mutex_unlock (&mutex);
		}
		//If the needs have all been met OR the process was declined, then exit
		if((need[n_customer][0] == 0 && need[n_customer][1] == 0 && need[n_customer][2] == 0) || exit){ // This is gonna drive dan crazy I can tell.
			break;
		}
	}
		//If it is the first run, then the resource was declined and the resources do not need to be released.
		//Otherwise, release the resources that were used.
		if(!firstRun){
			release_res(n_customer);
		}
}


int main(int argc, char *argv[])
{
	if ( argc != 4 ) { //Error if 3 resources are not supplied
        printf("ERROR: Please insert the values for 3 resources.\n");
    return 0;
    }
    else {
    	//Set the maximum resources
    	available[0] = atoi(argv[1]);
    	available[1] = atoi(argv[2]);
    	available[2] = atoi(argv[3]);
    	total[0] = atoi(argv[1]);
    	total[1] = atoi(argv[2]);
    	total[2] = atoi(argv[3]);

    }

    //Set the inUse of each resource to 0 and the initalNeed to 0.
    for(int i = 0; i < NUM_CUSTOMERS; i++){
    	for(int j = 0; j < NUM_RESOURCES; j++){
    		inUse[j] = 0;
    		initialNeed[i][j] = 0;
    	}
    }

    pthread_t *cust[NUM_CUSTOMERS];

    //Create the threads and get them working on the requesting and release resources
    for(int i = 0;i<NUM_CUSTOMERS;i++){
        pthread_create(&cust[i],NULL,procCust,(void *) i);
    }

    for(int i = 0;i<NUM_CUSTOMERS;i++){
        pthread_join(*cust[i],0);
    }
    

    return EXIT_SUCCESS;
}
