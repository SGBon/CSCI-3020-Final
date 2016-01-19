#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "bintree.h"

#define MAX 1024

node* curr;
node* base;

int main(){
<<<<<<< HEAD
  curr = (node*)malloc(sizeof(node));
  base = (node*)malloc(sizeof(node));
  base->left = NULL;
  base->right = NULL;


  proc baseproc;
  strcpy(baseproc.name,"NULL"); // initialize
  strcpy(baseproc.parent,"ignore"); // initialize
  baseproc.priority=0;
  baseproc.memory=0;
=======
  //node_t* base = NULL; //
  node_t* base = (node_t*)malloc(sizeof(node_t));
   base->left = NULL;
   base->right = NULL;


   proc* baseproc = (proc*)malloc(sizeof(baseproc));
   strcpy(baseproc->name,"NULL"); // initialize
   strcpy(baseproc->parent,"ignore"); // initialize
   baseproc->priority=0;
   baseproc->memory=0;

   base->process = *baseproc;
>>>>>>> 359a6ef7389a29d5750b765b0c5b64390f4a26ac

  base->process = baseproc;

  FILE* fl;
  fl = fopen("processes_tree.txt","r");
  char reader[MAX];
  const char* s = ", ";
  char *token;

<<<<<<< HEAD
  char parent[256];
  char name[256];
  char intbuff[256];
  int priority;
  int memory;
=======
  char parent[MAX] = {0};
  char name[MAX] = {0};
  char intbuff[MAX] = {0};
  int priority = 0;
  int memory = 0;

  while(fgets(reader,MAX,fl)!=NULL){
    node_t* curr;
    memset(&parent[0],0,sizeof(parent));
    memset(&name[0],0,sizeof(name));
    memset(&intbuff[0],0,sizeof(intbuff));
    priority = 0;
    memory = 0;
>>>>>>> 359a6ef7389a29d5750b765b0c5b64390f4a26ac

  while(fgets(reader,MAX,fl)){
    token = strtok(reader,s);
    for(int i = 0;i<4;i++){
        if(i == 0){ // parent
          strcpy(parent,token);
        }else if(i==1){ // name
          strcpy(name,token);
        }else if(i==2){ // prority
          strcpy(intbuff,token);
          priority = atoi(intbuff);
        }else if(i==3){ // memory
          strcpy(intbuff,token);
          memory = atoi(intbuff);
      }
      token = strtok(NULL,s);
    }

<<<<<<< HEAD
    proc newproc = (proc){.parent="",.name="",.priority=priority,.memory=memory};
    strcpy(newproc.parent,parent);
    strcpy(newproc.name,name);

    curr->process = newproc;

    adder(base,curr);
    //refresh();
=======
    newproc = (proc*)malloc(sizeof(proc));

    strcpy(newproc->parent,parent);
    strcpy(newproc->name,name);
    newproc->priority = priority;
    newproc->memory = memory;

    curr = (node_t*)malloc(sizeof(node_t));
    curr->left = NULL;
    curr->right = NULL;

    curr->process = *newproc;

    adder(&base,curr);
    refresh();
>>>>>>> 359a6ef7389a29d5750b765b0c5b64390f4a26ac
  }

  //fclose(fl);

  //print_tree(base);

  //decon(base);

  return 0;
}
