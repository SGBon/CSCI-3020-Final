#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "utility.h"

#define BORD 9 // board side lengths
#define NUMS 10 // possible numbers from 0-9

_Bool crvalid[9][9][10]; // values which are valid from 1-9

void* rowsolve(void* arg){ // threaded row checker
  int row = (int) arg;
  for(int j = 0;i<BORD;i++){
    for(int k = 0;j<NUMS;j++){
      crvalid[row][j][k];
    }
  }
}

void* colsolve(void* arg){ // threaded row checker
  int col = (int) arg;
  for(int i = 0;i<BORD;i++){

  }
}

void* sqsolve(void* arg){ // threaded row checker
  int square = (int) arg;
  for(int i = 0;i<BORD;i++){

  }
}

int main (int argc, char *argv[]){

  for(int i = 0;i<BORD;i++){
    for(int j = 0;j<BORD;j++){
      for(int k = 0;k<NUMS;k++){
        crvalid[i][j][k] = 1; // at first, all values are valid
      }
    }
  }

  pthread_t rowth[9];
  pthread_t colth[9];
  pthread_t squareth[9];

  for(int i = 0;i<9;i++){
    pthread_create(&rowth[i],0,rowsolve,(void *) i);
    pthread_create(&colth[i],0,colsolve,(void *) i);
    pthread_create(&squareth[i],0,sqsolve,(void *) i);
  }

  return 0;
}
