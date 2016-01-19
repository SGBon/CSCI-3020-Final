/*
* Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
*
* Copyright (C) 2015, <Dan Hope, Santiago Bonada, Colton Howe, Rhys Agombar>
* All rights reserved.
*
*/
#define _POSIX_SOURCE

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utility.h"
#include "hostd.h"

int alloc_mem(resources* res, int size,int priority){
  int loc;
  //if real time process allow it to use first 64
  if(priority==0){
    loc=0;
  }else{
    loc=64;
  }
  _Bool ok = 1;
  //Find a free memory block using first fit
  while(loc<MEMORY){
    ok=1;
    if(loc+size>MEMORY){ return -1; }
    for(int i=loc;i<loc+size;i++){
      if(res->memory[i]==1){
        ok=0;
        loc=i;
        break;
      }
    }
    //if block found allocate it
    if(ok){
      for(int x=loc;x<loc+size;x++){
        res->memory[x]=1;
      }
      return loc;
    }else{
      for(int j=loc;j<MEMORY;j++){
        if(res->memory[j]==0){
          loc=j;
          break;
        }else if(j==MEMORY-1){
          return -1;
        }
      }
    }
  }
  return -1;
}

void free_mem(resources* res,int address,int size){
  //free up block of memory at address
  for(int i=address;i<address+size;i++){
    res->memory[i]=0;
  }
}

void free_resources(resources* res, proc process){
  //Free memory
  free_mem(res,process.address,process.memory);
  //Free scanners
  int alloc=process.scanners;
  for(int i=0;i<SCANNERS;i++){
    if(!alloc) break;
    if(res->scanners[i]==1){
      res->scanners[i]=0;
      alloc--;
    }
  }
  //free modems
  alloc=process.modems;
  for(int i=0;i<MODEMS;i++){
    if(!alloc) break;
    if(res->modems[i]==1){
      res->modems[i]=0;
      alloc--;
    }
  }
  //free drives
  alloc=process.drives;
  for(int i=0;i<DRIVES;i++){
    if(!alloc) break;
    if(res->drives[i]==1){
      res->drives[i]=0;
      alloc--;
    }
  }
  //free printers
  alloc=process.printers;
  for(int i=0;i<PRINTERS;i++){
    if(!alloc) break;
    if(res->printers[i]==1){
      res->printers[i]=0;
      alloc--;
    }
  }
}

int alloc_resources(resources* res, proc* process){
  //number of free resources
  int free_printers=0;
  int free_modems=0;
  int free_drives=0;
  int free_scanners=0;

  //count free scanners
  for(int i=0;i<SCANNERS;i++){
    if(res->scanners[i]==0){
      free_scanners++;
    }
  }
  //if not enough free scanners for current job break allocation
  if(free_scanners<process->scanners) return 0;

  //count modems
  for(int i=0;i<MODEMS;i++){
    if(res->modems[i]==0){
      free_modems++;
    }
  }
  if(free_modems<process->modems) return 0;

  //count drives
  for(int i=0;i<DRIVES;i++){
    if(res->drives[i]==0){
      free_drives++;
    }
  }
  if(free_drives<process->drives) return 0;

  //count printers
  for(int i=0;i<PRINTERS;i++){
    if(res->printers[i]==0){
      free_printers++;
    }
  }
  if(free_printers<process->printers) return 0;

  //check to see if memory can be allocated now that enough resources are available
  int mem_address = alloc_mem(res,process->memory,process->priority);
  if(mem_address == -1){
    return 0;
  }else{
    //if memory is good allocate all resources
    process->address=mem_address;
    int alloc=process->scanners;
    for(int i=0;i<SCANNERS;i++){
      if(!alloc) break;
      if(res->scanners[i]==0){
        res->scanners[i]=1;
        alloc--;
      }
    }

    alloc=process->modems;
    for(int i=0;i<MODEMS;i++){
      if(!alloc) break;
      if(res->modems[i]==0){
        res->modems[i]=1;
        alloc--;
      }
    }

    alloc=process->drives;
    for(int i=0;i<DRIVES;i++){
      if(!alloc) break;
      if(res->drives[i]==0){
        res->drives[i]=1;
        alloc--;
      }
    }

    alloc=process->printers;
    for(int i=0;i<PRINTERS;i++){
      if(!alloc) break;
      if(res->printers[i]==0){
        res->printers[i]=1;
        alloc--;
      }
    }
  }
  return 1;
}

void load_dispatch(char *dispatch_file){
  //Open file aznd ready for reading
  FILE *f;
  f = fopen(dispatch_file,"r");
  char line[1024];
  const char* s = ", ";
  char *token;

  int t_data[8];


  //Read in CSV file
  while(fgets(line,1024,f)){

    token = strtok(line, s);

    for(int i=0;i<8;i++){
      t_data[i]=atoi(token);
      token = strtok(NULL, s);
    }

    //Create process
    proc new_proc = (proc){.arrival_time=t_data[0], .priority=t_data[1], .runtime=t_data[2], .memory=t_data[3],.printers=t_data[4], .scanners=t_data[5], .modems=t_data[6], .drives=t_data[7],.pid=0, .address=0, .suspended=false};

    //push process to dispatcher
    push(q_dispatch,new_proc);
  }

}

int run_proc(proc* process){
  int status;
  pid_t pid;

  //if this is a new process create it
  if(!process->suspended && process->pid==0){
    pid=fork();
    if(pid == 0){
      execlp("./process","./process",(char*) NULL);
    }else if(pid<0){
      // fork failed to fork
      fprintf(stderr,"Fork failed.\n");
      status = -1;
    }else{
      //Assign process ID
      process->pid=pid;
      //Display process information
      printf("Creating - PID %d with priority %d for %d(s). Using %d Bytes of memory at &%d\nResources Requested: (%d, %d, %d, %d)\n"
      , process->pid, process->priority, process->runtime, process->memory, process->address, process->printers,process->scanners,process->modems,process->drives);
      //run as real time
      if(process->priority==0){
        sleep(process->runtime);
        kill(pid,SIGINT);
        waitpid(process->pid,&status,WUNTRACED);
        free_resources(&res_avail,*process);
        return 0;
      }else if(process->runtime>1&&process->priority>0){
        //run as non-realtime
        sleep(1);
        kill(pid,SIGTSTP);
        waitpid(process->pid,&status,WUNTRACED);
        process->runtime--;
        process->suspended=true;
        return 1;
      }else{
        //run for only one cycle then terminate
        sleep(process->runtime);
        kill(pid,SIGINT);
        waitpid(process->pid,&status,WUNTRACED);
        free_resources(&res_avail,*process);
        return 0;
      }
    }
  }else{
    //run for cycle
    if(process->runtime>1){
      kill(process->pid,SIGCONT);
      sleep(1);
      kill(process->pid,SIGTSTP);
      waitpid(process->pid,&status,WUNTRACED);
      process->runtime--;
      process->suspended=true;
      return 1;
    }else{
      //run for 1 cycle and terminate
      kill(process->pid,SIGCONT);
      sleep(process->runtime);
      kill(process->pid,SIGINT);
      waitpid(process->pid,&status,WUNTRACED);
      free_resources(&res_avail,*process);
      return 0;
    }
  }
}
