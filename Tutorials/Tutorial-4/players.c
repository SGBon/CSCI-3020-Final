/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"

// Returns true if the player name matches one of the existing players
bool player_exists(player *players, char *name)
{
  bool exist = false;
  int arrLen = player_count(players);
  for(int i = 0;i < arrLen;i++){
    if(players[i].name == name){
      exist = true;
    }
  }
    return exist;
}

// Updates the score for that player given their name
void update_score(player *players, char *name, int score)
{
  int arrLen = player_count(players);
  if(player_exists(players, name)){
    for (int i = 0;i < arrLen; i++){
      if(players[i].name == name){
        players[i].score += score;
      }
    }
  }
}

int player_count(player *players){
  return sizeof players / sizeof players[0];
}
