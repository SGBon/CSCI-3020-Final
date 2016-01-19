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

int main(void){

  queue* fifo=(queue*)malloc(sizeof(queue*));
  fifo->head=NULL;
  fifo->tail=NULL;

  //Open file and ready for reading
  FILE *f;
  f = fopen("processes.txt","r");
  char line[1024];
  const char* s=", ";
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
    proc new_proc = (proc){.name="",.priority=t_data[1],.pid=t_data[2],.runtime=t_data[3]};
    strcpy(new_proc.name,t_name);
    push(fifo,new_proc);

  }

  node* cur_node = fifo->head;
  while(cur_node){
    printf("%s, %d, %d, %d\n",cur_node->process.name,cur_node->process.priority,cur_node->process.pid,cur_node->process.runtime);
    cur_node=cur_node->next;
  }

}
