/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <Dan Hope, Santiago Bonada, Colton Howe, Rhys Agombar>
 * All rights reserved.
 *
 */
#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdbool.h>

//Constants for system values
#define MEMORY 1024
#define PRINTERS 2
#define SCANNERS 1
#define MODEMS 1
#define DRIVES 2

//Resources structure
typedef struct{
  int printers[PRINTERS];
  int scanners[SCANNERS];
  int modems[MODEMS];
  int drives[DRIVES];
  int memory[MEMORY];
}resources;

//Process structure
typedef struct{
  int arrival_time;
  int priority;
  int runtime;
  int memory;
  int printers;
  int scanners;
  int modems;
  int drives;

  int pid;
  int address;
  bool suspended;
}proc;

//Check for free memory and allocate for process
extern int alloc_mem(resources* res, int size,int priority);
//Free currently used memory
extern void free_mem(resources* res,int address,int size);
//Run a process
extern int run_proc(proc* process);
//Load in dispatch list from a CSV
extern void load_dispatch(char *dispatch_file);
//check for free resources and then allocate
extern int alloc_resources(resources* res, proc* process);
//Free all allocated resources
extern void free_resources(resources* res, proc process);

#endif /* UTILITY_H_ */
