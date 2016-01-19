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
  char parent[256];
  char name[256];
  int priority;
  int memory;
}proc;

typedef struct proc_tree_ {
  proc process;
  struct proc_tree_* left;
  struct proc_tree_* right;
}proc_tree;

proc_tree* root;

proc_tree* lookup(proc_tree* node, char* target) {
  if (node == NULL) {
    return(NULL);
  }
  else {
    if (strcmp(target,node->process.name)==0) return(node);
    else {
      if (strcmp(target,node->process.name)>=0) return(lookup(node->left, target));
      else return(lookup(node->right, target));
    }
  }
}

void add_proc(proc_tree* new_proc){
  if(!root){
    if(strcmp(new_proc->process.parent,"NULL")==0){
      root=new_proc;
      printf("root is: %s\n",root->process.name);
    }
  }else{
    if(strcmp(new_proc->process.parent,new_proc->process.name)<=0){
      proc_tree* parent = lookup(root,new_proc->process.parent);
      parent->left=new_proc;
      printf("added %s to %s on left\n",new_proc->process.name,parent->process.name);
    }else{
      proc_tree* parent = lookup(root,new_proc->process.parent);
      parent->right=new_proc;
      printf("added %s to %s on right\n",new_proc->process.name,parent->process.name);
    }
  }
}



int main(void){

  root = (proc_tree*)malloc(sizeof(proc_tree));
  root=NULL;

  //Open file aznd ready for reading
  FILE *f;
  f = fopen("processes_tree.txt","r");
  char line[1024];
  const char* s = ", ";
  char *token;

  char* t_parent[256];
  char* t_name[256];
  int* t_data[4];

  //Read in CSV file
  while(fgets(line,1024,f)){

    token = strtok(line, s);

    for(int i=0;i<4;i++)
    {
      if(i==0)
      {
        strcpy(t_parent,token);
      } else if(i==1){
        token = strtok(NULL, s);
        strcpy(t_name,token);
      } else{
        token = strtok(NULL, s);
        t_data[i]=atoi(token);
      }
    }

    //Create process
    proc new_proc = (proc){.parent="",.name="",.priority=t_data[2],.memory=t_data[3]};
    strcpy(new_proc.parent,t_parent);
    strcpy(new_proc.name,t_name);
    proc_tree* new_tree = (proc_tree*)malloc(sizeof(proc_tree));
    new_tree->process = new_proc;
    new_tree->left=NULL;
    new_tree->right=NULL;
    printf("Adding: %s\n",new_proc.name);
    add_proc(new_tree);
  }

}
