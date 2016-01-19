#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"

int q_size(queue* fifo){
  int size=0;
  node* cur = fifo->head;
  while(cur){
    size++;
    cur = cur->next;
  }
  return size;
}

void push(queue* fifo,proc process){
  node* temp = (node*)malloc(sizeof(node));
  temp->process=process;
  temp->next=NULL;
  if(fifo->head==NULL){
    fifo->head=temp;
    fifo->tail=temp;
  }else if(fifo->head==fifo->tail){
    fifo->head->next = temp;
    fifo->tail = temp;
  }else{
    fifo->tail->next = temp;
    fifo->tail = temp;
  }

}

proc *pop(queue* fifo){
  if(fifo->head!=NULL){
    proc* temp = &fifo->head->process;
    fifo->head=fifo->head->next;
    return temp;
  }else{
    return NULL;
  }
}
