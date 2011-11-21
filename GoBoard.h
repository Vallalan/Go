/*   Author: Alan Gieske
             (Allen Hsia)
     Project: Go
     
     Description: Assignment for programming languages course
 */

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
  Player emptyOwner;
  Stone *** grid; //2d array of stone pointers

 public:
  GoBoard( int width, int height );

  ~GoBoard();

  void printboard();
  int step(Player player);
  Player scoreArea();

 private:
  bool isOnBoard( int x, int y );
  int countNeighbors( Player target, int x, int y );
  bool effectEnemies( Player enemy, int x, int y );
  int countLiberties( Player player, int x, int y );
  void clearMarks();
  void clearGroup( Player player, int x, int y );
  int getSize( Player player, int x, int y );
  int scoreEmpty( int x, int y );
  Group getGroup( int x, int y, Group currGroup );
};
