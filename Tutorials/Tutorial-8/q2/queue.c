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

proc *delete_name(queue* fifo,char* name){
  node* cur_node = fifo->head;
  node* last_node = cur_node;
  while(cur_node){
    if(!strcmp(cur_node->process.name,name)){
      last_node->next=cur_node->next;
      return &cur_node->process;
    }
    last_node=cur_node;
    cur_node=cur_node->next;

  }
  return NULL;
}

proc *delete_pid(queue* fifo,int pid){
  node* cur_node = fifo->head;
  node* last_node = cur_node;
  while(cur_node){
    if(cur_node->process.pid==pid){
      last_node->next=cur_node->next;
      return &cur_node->process;
    }
    last_node=cur_node;
    cur_node=cur_node->next;
  }
  return NULL;
}
