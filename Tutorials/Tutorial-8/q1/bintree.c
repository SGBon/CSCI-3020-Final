#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "bintree.h"

_Bool added = 0;
_Bool exist = 0;

// define functions here

void dfs(node* tree,char parent[256]){
  if(strcmp(tree->process.name,parent)==0){
    exist = 1;
  }else{
    if(tree->left!=NULL)
    dfs(tree->left,parent);
    else if (tree->right!=NULL)
    dfs(tree->right,parent);
  }
}



void adder(node** tree,node* new){
  // if we are at a null node, then set the node here
  if(!(*tree)){
    if(!added){
      *tree = new;
      added = 1;
      return;
    }
  }else{
    // check if the parent matches, if not continue through tree
    if(strcmp((*tree)->process.name,new->process.parent)==0){
      if((*tree)->left==NULL){
        adder(&(*tree)->left,new);
      }else if((*tree)->right==NULL){
        adder(&(*tree)->right,new);
      }
    }else{
      if((*tree)->left!=NULL){
        adder(&(*tree)->left,new);
      }
      if ((*tree)->right!=NULL){
        adder(&(*tree)->right,new);
      }
    }
  }
}

void refresh(){
  added = 0;
}

void decon(node* tree){
  if(tree != NULL){
    decon(tree->left);
    decon(tree->right);
    //free(tree);
  }
}

void print_tree(node* tree){

  if(tree!=NULL){
    if(tree->left!=NULL){
      fprintf(stdout,"%s left:--> %s\n",tree->process.name,tree->left->process.name);
    }
    if(tree->right!=NULL){
      fprintf(stdout,"%s right:--> %s\n",tree->process.name,tree->right->process.name);
    }
    print_tree(tree->left);
    print_tree(tree->right);
  }
}
