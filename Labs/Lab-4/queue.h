#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include "utility.h"

//node struct for holding a process in queue
typedef struct node_ {
  proc process;
  struct node_* next;
}node;

//struct for holding a queue
typedef struct{
  node* head;
  node* tail;
}queue;

//find size of queue
extern int q_size(queue* fifo);
//push new process onto queue
extern void push(queue* fifo,proc process);
//pop process from queue
extern proc *pop(queue* fifo);

#endif
