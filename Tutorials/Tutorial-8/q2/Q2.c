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

#define MEMORY 1024

int avail_mem[MEMORY];
queue* priority;
queue* secondary;

int check_mem(int* address,int size){
  int loc=0;
  _Bool ok = 1;
  while(loc<MEMORY){
    ok=1;
    if(loc+size>MEMORY){ return 0;}
    for(int i=loc;i<loc+size;i++){
      if(avail_mem[i]==1){
        ok=0;
        loc=i;
        break;
      }
    }
    if(ok){
      *address = loc;
      for(int x=loc;x<loc+size;x++){
        avail_mem[x]=1;
      }
      return 1;
    }else{
      for(int j=loc;j<MEMORY;j++){
        if(avail_mem[j]==0){
          loc=j;
          break;
        }else if(j==MEMORY-1){
          return 0;
        }
      }
    }
  }
  return 0;
}

void free_mem(int address,int size){
  for(int i=address;i<address+size;i++){
    avail_mem[i]=0;
  }
}

int run_proc(proc* process){
  int status;
  pid_t pid;

  if(!process->suspended && process->pid==0){
    pid=fork();
    if(pid == 0){
      execlp("./process",(char*) NULL);
    }else if(pid<0){
      // fork failed to fork
      fprintf(stderr,"Fork failed.\n");
      status = -1;
    }else {
      process->pid=pid;

      printf("%s executing at PID %d with priority %d. Using %d Bytes of memory for %d second(s)\n"
      ,process->name, process->pid, process->priority, process->memory, process->runtime);

      if(process->runtime>1&&process->priority>0){
        sleep(1);
        kill(pid,SIGTSTP);
        waitpid(process->pid,&status,WUNTRACED);
        process->runtime--;
        process->suspended=true;
        return 1;
      }else{
        sleep(process->runtime);
        kill(pid,SIGINT);
        waitpid(process->pid,&status,WUNTRACED);
        free_mem(process->address,process->memory);
        return 0;
      }
    }
  }else{
    printf("%s executing at PID %d with priority %d. Using %d Bytes of memory for %d second(s)\n"
    ,process->name, process->pid, process->priority, process->memory, process->runtime);

    if(process->runtime>1){
      kill(process->pid,SIGCONT);
      sleep(1);
      kill(process->pid,SIGTSTP);
      waitpid(process->pid,&status,WUNTRACED);
      process->runtime--;
      process->suspended=true;
      return 1;
    }else{
      kill(process->pid,SIGCONT);
      sleep(process->runtime);
      kill(process->pid,SIGINT);
      waitpid(process->pid,&status,WUNTRACED);
      free_mem(process->address,process->memory);
      return 0;
    }
  }
}

int main(void){
  priority = (queue*)malloc(sizeof(queue*));
  priority->head=NULL;
  priority->tail=NULL;
  secondary = (queue*)malloc(sizeof(queue*));
  secondary->head=NULL;
  secondary->tail=NULL;
  for(int i=0;i<MEMORY;i++){
    avail_mem[i]=0;
  }

  //Open file aznd ready for reading
  FILE *f;
  f = fopen("processes_q5.txt","r");
  char line[1024];
  const char* s = ", ";
  char *token;

  char* t_name[256];
  int* t_data[4];

  //Read in CSV file
  while(fgets(line,1024,f)){

    token = strtok(line, s);

    for(int i=0;i<4;i++)
    {
      if(i==0)
      {
        strcpy(t_name,token);
      } else {
        token = strtok(NULL, s);
        t_data[i]=atoi(token);
      }
    }

    //Create process
    proc new_proc = (proc){.name="",.priority=t_data[1],.pid=0,.address=0,.memory=t_data[2],.runtime=t_data[3],.suspended=false};
    strcpy(new_proc.name,t_name);
    if(new_proc.priority==0){
      push(priority,new_proc);
    }else{
      push(secondary,new_proc);
    }

  }

  //Pop priority
  proc* pri_proc = pop(priority);
  while(pri_proc){
    if(check_mem(&pri_proc->address,pri_proc->memory)){
      run_proc(pri_proc);
    }else{
      push(priority,*pri_proc);
    }
    pri_proc = pop(priority);
  }

  //Pop secondary
  proc* sec_proc = pop(secondary);
  while(sec_proc){
    if(!sec_proc->suspended){
      if(check_mem(&sec_proc->address,sec_proc->memory)){
        if(run_proc(sec_proc)){
          push(secondary,*sec_proc);
        }
      }else{
        push(secondary,*sec_proc);
      }
    }else{
      if(run_proc(sec_proc)){
        push(secondary,*sec_proc);
      }
    }
    sec_proc = pop(secondary);
  }

  free(priority);
  free(secondary);
}
