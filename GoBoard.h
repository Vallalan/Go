/*   Author: Alan Gieske
     Project: Go
     
     Description: Assignment for programming languages course
 */

#include<iostream>

//for the stone class and the Player enum type
#include "Stone.h"

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
  void effectEnemies( Player enemy, int x, int y );
  int countLiberties( Player player, int x, int y );
  void clearMarks();
  void clearGroup( Player player, int x, int y );
  int getSize( Player player, int x, int y );
  int scoreEmpty( int x, int y );
};
