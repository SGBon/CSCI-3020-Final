/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Colton Howe, Rhys Agombar, Santiago Bonada, Dan Hope
 * All rights reserved.
 * 
 */
#ifndef BANKER_H_
#define BANKER_H_

#include <stdbool.h>

// Request resources, returns true if successful
extern bool request_res(int n_customer, int request[]);

// Release resources, returns true if successful
extern bool release_res(int n_customer);

#endif /* BANKER_H_ */