/*   Author: Alan Gieske
             (Allen Hsia)
     Project: Go
     
     Description: 
           Header file for main game controller
 */
#ifndef GOBOARD_H_
#define GOBOARD_H_
#include<iostream>

//for the stone class and the Player enum type
#include "Stone.h"

typedef struct Group_t{
  int size;
  int liberties;
  Player player;
  Player owner;
} Group;


class GoBoard 
{

 private:
  int height;
  int width;
  Stone *** grid; //2d array of stone pointers
  int scoreBlack;
  int scoreWhite;

 public:
  GoBoard( int width, int height );

  ~GoBoard();

  void printboard();
  int step(Player player, int x, int y);
  Player scoreArea();
  Player scoreTerritory();
  bool isValidMove( int x, int y );
  char getPlayer( int x, int y );
  int getScoreBlack();
  int getScoreWhite();
 private:
  
  bool isOnBoard( int x, int y );
  bool isPlayer( Player target, int x, int y );
  bool isKo( int x, int y );  
  int countNeighbors( Player target, int x, int y );
  bool effectEnemies( Player enemy, int x, int y );
  void clearMarks();
  void clearGroup( Player player, int x, int y );
  Group getGroup( int x, int y, Group currGroup );
};

#endif //GOBOARD_H_
