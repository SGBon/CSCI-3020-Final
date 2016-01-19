/*
* Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
*
* Copyright (C) 2015, <Dan Hope, Santiago Bonada, Colton Howe, Rhys Agombar>
* All rights reserved.
*
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"

int main(int argc, char *argv[]){
  //Init queues
  q_dispatch = (queue*)malloc(sizeof(queue*));
  q_dispatch->head=NULL;
  q_dispatch->tail=NULL;

  q_real = (queue*)malloc(sizeof(queue*));
  q_real->head=NULL;
  q_real->tail=NULL;

  q_1 = (queue*)malloc(sizeof(queue*));
  q_1->head=NULL;
  q_1->tail=NULL;

  q_2 = (queue*)malloc(sizeof(queue*));
  q_2->head=NULL;
  q_2->tail=NULL;

  q_3 = (queue*)malloc(sizeof(queue*));
  q_3->head=NULL;
  q_3->tail=NULL;


  //Init resources
  for(int i=0;i<PRINTERS;i++){res_avail.printers[i]=0;}
  for(int i=0;i<SCANNERS;i++){res_avail.scanners[i]=0;}
  for(int i=0;i<MODEMS;i++){res_avail.modems[i]=0;}
  for(int i=0;i<DRIVES;i++){res_avail.drives[i]=0;}
  for(int i=0;i<MEMORY;i++){res_avail.memory[i]=0;}

  // Load the dispatchlist adds to queue as well
  load_dispatch(argv[1]);

  int tick =0;
  //Start dispatching
  while(true){
    //check dispatch list for processes that have arrived
    node* dispatch_node = q_dispatch->head;
    while(dispatch_node){
      if(dispatch_node->process.arrival_time==tick){
        //Push process to appropriate queue
        switch(dispatch_node->process.priority){
          case 0:
          push(q_real,dispatch_node->process);
          break;
          case 1:
          push(q_1,dispatch_node->process);
          break;
          case 2:
          push(q_2,dispatch_node->process);
          break;
          case 3:
          push(q_3,dispatch_node->process);
          break;
        }
      }
      dispatch_node=dispatch_node->next;
    }

    //Run processes
    proc* get_proc;
    bool find_proc = true;
    int count;

    //Real Time Queue
    count = q_size(q_real);
    //loop through queue if there is something in queue and no job has been found
    while(find_proc&&count>0){
      get_proc = pop(q_real);
      //check if process can be allocated
      if(alloc_resources(&res_avail,get_proc)){
        //run
        run_proc(get_proc);
        find_proc=false;
      }else{
        push(q_real,*get_proc);
        count--;
      }
    }

    //1st Queue
    count = q_size(q_1);
    //loop through queue if there is something in queue and no job has been found
    while(find_proc&&count>1){
      get_proc = pop(q_1);
      //Check if process can be allocated
      if(alloc_resources(&res_avail,get_proc)){
        //run if not completed during run push to next queue
        if(run_proc(get_proc))
        push(q_2,*get_proc);
        find_proc=false;
      }else{
        //if resources not available push back onto queue
        push(q_1,*get_proc);
        count--;
      }
    }

    //2nd Queue
    count = q_size(q_2);
    //loop through queue if there is something in queue and no job has been found
    while(find_proc&&count>0){
      get_proc = pop(q_2);
      //if process is a new process
      if(!get_proc->suspended){
        //check if it can be allocated
        if(alloc_resources(&res_avail,get_proc)){
          //run if not completed during run push to next queue
          if(run_proc(get_proc))
          push(q_3,*get_proc);
          find_proc=false;
        }else{
          //if resources not available push back onto queue
          push(q_2,*get_proc);
          count--;
        }
      }else{
        //if processes has already been allocated just run it some more
        if(run_proc(get_proc))
        push(q_3,*get_proc);
        find_proc=false;
      }
    }

    //3rd Qeue
    count = q_size(q_3);
    while(find_proc&&count>0){
      get_proc = pop(q_3);
      if(!get_proc->suspended){
        if(alloc_resources(&res_avail,get_proc)){
          if(run_proc(get_proc))
          push(q_3,*get_proc);
          find_proc=false;
        }else{
          push(q_3,*get_proc);
          count--;
        }
      }else{
        if(run_proc(get_proc))
        push(q_3,*get_proc);
        find_proc=false;
      }
    }

    //if no process was able to be run either system is hung or there are no jobs left
    if(find_proc){
      printf("TERMINATING: Could not find anymore processes\n");
      break;
    }

    //increase tick count on dispatcher
    tick++;
  }

  return EXIT_SUCCESS;
}
