#include <stdbool.h>

typedef struct{
  char name[256];
  int priority;
  int pid;
  int address;
  int memory;
  int runtime;
  bool suspended;
}proc;

typedef struct node_ {
  proc process;
  struct node_* next;
}node;

typedef struct{
  node* head;
  node* tail;
}queue;

extern void push(queue* fifo,proc process);
extern proc *pop(queue* fifo);
extern proc *delete_name(queue* fifo,char* name);
extern proc *delete_pid(queue* fifo,int pid);
