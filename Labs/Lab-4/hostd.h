/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <Dan Hope, Santiago Bonada, Colton Howe, Rhys Agombar>
 * All rights reserved.
 *
 */
#ifndef HOSTD_H_
#define HOSTD_H_

#include "queue.h"
#include "utility.h"

//Dispatch queue
queue* q_dispatch;

//Job queues
queue* q_real;
queue* q_1;
queue* q_2;
queue* q_3;

//Resources
resources res_avail;

#endif /* HOSTD_H_ */
