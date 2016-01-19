#include "utility.h"

int* load(void){
  FILE *f;
  f = fopen("puzzle.txt","r");
  char line[256];

  int arr[9][9][10];

  for (int y = 0; y < 9; y++){
    fgets(line,MAX_LEN,f);
    for (int x = 0; x < 9; x++){
      sscanf(line,"%d",arr[x][y][0]);
      for (int z = 1; z < 10; z++){
        arr[x][y][z] = 0;
      }
    }
  }

  return arr;

}