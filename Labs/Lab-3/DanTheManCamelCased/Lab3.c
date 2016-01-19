#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Struct to hold a pair of integers
typedef struct {
  int x;
  int y;
}vector;

//Threads
pthread_t t_col;
pthread_t t_row;
pthread_t t_box;

pthread_mutex_t mutex;

//Solved Board
int* sudoku_board[9][9];
//Working Board
_Bool* sudoku_solution[9][9][10];

//Load in the puzzle from text file
void load(int* arr[][9]){
  FILE *f;
  f = fopen("puzzle.txt","r");

  //0 intilize array
  for(int y=0;y<9;y++){
    for(int x=0;x<9;x++){
      arr[x][y]=0;
    }
  }

  //fill array with values from text file
  for (int y = 0; y < 9; y++){
    for (int x = 0; x < 9; x++){
      fscanf(f,"%d",&arr[x][y]);
    }
  }

  //set up working board for solving
  for (int y = 0; y < 9; y++){
    for (int x = 0; x < 9; x++){
      int index=arr[x][y];
      if(index!=0){
        sudoku_solution[x][y][index]=1;
      }else{
        for(int z=0;z<10;z++){
          sudoku_solution[x][y][z]=1;
        }
      }
    }
  }
  fclose(f);
}

//Check row for potential values
void* check_row(void* arg){
  vector* pos = arg;
  int xx=pos->x;
  int yy=pos->y;

  for(int x=0;x<9;x++){
    pthread_mutex_lock(&mutex);
    int position = sudoku_board[x][yy];
    sudoku_solution[xx][yy][position]=0;
    pthread_mutex_unlock(&mutex);
  }
}

//Check column for potential values
void *check_col(void* arg){
  vector* pos = arg;
  int xx=pos->x;
  int yy=pos->y;
  for(int y=0;y<9;y++){
    pthread_mutex_lock(&mutex);
    int position = sudoku_board[xx][y];
    sudoku_solution[xx][yy][position]=0;
    pthread_mutex_unlock(&mutex);
  }
}

//Check 3x3 box for potential values
void* check_box(void* arg){
  vector* pos = arg;
  int xx=pos->x;
  int yy=pos->y;

  int start_y=(yy / 3)*3;
  int start_x=(xx / 3)*3;
  for(int y=start_y;y<start_y+3;y++){
    for(int x=start_x;x<start_x+3;x++){
      pthread_mutex_lock(&mutex);
      int position = sudoku_board[x][y];
      if(position){
      }
      sudoku_solution[xx][yy][position]=0;
      pthread_mutex_unlock(&mutex);
    }
  }
}

//check the current cell for all potentials
void check_pos(int xx,int yy){
  vector* pos = (vector*)malloc(sizeof(vector*));
  pos->x=xx;
  pos->y=yy;
  //dispatch a thread for possible values in row/col/box
  pthread_create(&t_col,NULL,check_col,(void *)pos);
  pthread_create(&t_row,NULL,check_row,(void *)pos);
  pthread_create(&t_box,NULL,check_box,(void *)pos);
  pthread_join(t_row,0);
  pthread_join(t_box,0);
  pthread_join(t_col,0);
  free(pos);

  //cell has been checked
  sudoku_solution[xx][yy][0]=0;
}

int main (void){

  //Init mutex
  if (pthread_mutex_init(&mutex, NULL) != 0){
    printf("\n mutex init failed\n");
  }

  load(&sudoku_board);
  _Bool solved=0;

  //Display unsolved
  printf("------Original------\n");
  for (int j = 0; j < 9; j++){
    for (int k = 0; k < 9; k++){
      printf("%d ",sudoku_board[k][j]);
    }
    printf("\n");
  }

  //Keep solving puzzle until fully solved
  while(!solved){
    solved=1;

    //Check position of every cell
    for (int j = 0; j < 9; j++){
      for (int k = 0; k < 9; k++){
        check_pos(k,j);
      }
    }

    //Check each cell and see if there is a single potential value
    for (int j = 0; j < 9; j++){
      for (int k = 0; k < 9; k++){
        if(sudoku_board[k][j]==0){
          int single=0;
          for(int l=1;l<10;l++){
            if(sudoku_solution[k][j][l]==1){
              if(single==0){
                single = l;
              }else{
                single=0;
                break;
              }
            }
          }
          //If there is a single value then set that as the value on solved board
          if(single){
            sudoku_board[k][j]=single;
          }
        }
      }
    }

    //Go through solved board to see if it is fully solved
    for(int j=0;j<9;j++){
      for(int k=0;k<9;k++){
        if(sudoku_board[k][j]==0){
          solved=0;
        }
      }
    }
  }

  //Print solution to text and display completed board

  FILE* fp;
  fp = fopen("solution.txt", "w+");

  printf("--------Solved--------\n");
  for (int j = 0; j < 9; j++){
    for (int k = 0; k < 9; k++){
      printf("%d ",sudoku_board[k][j]);
      fprintf(fp,"%d ",sudoku_board[k][j]);
    }
    printf("\n");
    fprintf(fp,"\n");
  }
  fclose(fp);

  printf("Solution written to solution.txt\n");

  pthread_mutex_destroy(&mutex);
  return 0;
}
