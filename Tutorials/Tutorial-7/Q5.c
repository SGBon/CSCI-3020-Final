#define _XOPEN_SOURCE 600

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct{
  char name[256];
  int priority;
  int pid;
  int runtime;
}proc;

typedef struct node_ {
  proc process;
  struct node_* next;
}node;

typedef struct{
  node* head;
  node* tail;
}queue;

void push(queue* priority_queue,proc process){
  node* temp = (node*)malloc(sizeof(node));
  temp->process=process;
  temp->next=NULL;
  //Empty Queue
  if(priority_queue->head==NULL){
    printf("adding first node %d\n",temp->process.priority);
    priority_queue->head=temp;
    priority_queue->tail=temp;
    //single item
  }else if(priority_queue->head==priority_queue->tail){
    if(temp->process.priority>priority_queue->head->process.priority){
      //printf("adding 2nd node %d ahead\n",temp->process.priority);
      priority_queue->head->next = temp;
      priority_queue->tail = temp;
    }else{
      //printf("adding 2nd node %d behind\n",temp->process.priority);
      temp->next=priority_queue->head;
      priority_queue->tail=priority_queue->head;
      priority_queue->head=temp;
    }
    //two or more
  }else{
    node* cur_node=priority_queue->head;
    node* last_node=priority_queue->head;
    while(cur_node){
      //if node being pushed has higher priority
      if(temp->process.priority<cur_node->process.priority){
        //if that priority is highest
        if(priority_queue->head==cur_node){
          //make head new process
          priority_queue->head=temp;
        }
        //put it between last node and current node
        last_node->next=temp;
        temp->next=cur_node;
        //printf("adding priority %d ahead %d\n",temp->process.priority,cur_node->process.priority);
        return;
      }else{
        last_node=cur_node;
        cur_node=cur_node->next;
      }
    }
    //lower priority then everything add to end
    //printf("adding %d to end\n",temp->process.priority);
    last_node->next=temp;
    priority_queue->tail=temp;
  }

}

proc *pop(queue* priority_queue){
  if(priority_queue->head!=NULL){
    proc* temp = &priority_queue->head->process;
    priority_queue->head=priority_queue->head->next;
    return temp;
  }else{
    return NULL;
  }
}

proc *delete_name(queue* priority_queue,char* name){
  node* cur_node = priority_queue->head;
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

proc *delete_pid(queue* priority_queue,int pid){
  node* cur_node = priority_queue->head;
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

void run_proc(proc* process){
  int status;
  pid_t pid;
  pid=fork();
  if(pid == 0){
    execlp("./process",(char*) NULL);
  }else if(pid<0){
    // fork failed to fork
    fprintf(stderr,"Fork failed.\n");
    status = -1;
  }else {
    process->pid=pid;
    sleep(process->runtime);
    kill(pid,SIGINT);
    waitpid(pid,&status,0);
    printf("%s executed at PID %d with priority %d for %d second(s)\n",process->name,process->pid,process->priority,process->runtime);
  }
}

int main(void){
  queue* priority_queue=(queue*)malloc(sizeof(queue*));
  priority_queue->head=NULL;
  priority_queue->tail=NULL;

  //Open file aznd ready for reading
  FILE *f;
  f = fopen("processes_q5.txt","r");
  char line[1024];
  const char* s=", ";
  char *token;

  char* t_name[256];
  int* t_data[3];


  //Read in CSV file
  while(fgets(line,1024,f)){

    token = strtok(line, s);

    for(int i=0;i<3;i++)
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
    proc new_proc = (proc){.name="",.priority=t_data[1],.pid=0,.runtime=t_data[2]};
    strcpy(new_proc.name,t_name);
    printf("Creating Process Priority: %d\n",new_proc.priority);
    push(priority_queue,new_proc);

  }

  proc* process = pop(priority_queue);
  while(process){
    run_proc(process);
    process=pop(priority_queue);
  }

  free(priority_queue);
}
