

#include<iostream>

#include "Stone.h"

typedef enum Player_t {
  BLACK = 'B',
  WHITE = 'W',
  EMPTY = '.'
} Player;

class GoBoard 
{

 private:
  int height;
  int width;
  Stone *** grid; //2d array of pointers to stones

 public:
  GoBoard( int width, int height );

  ~GoBoard();

  void printboard();
  int step(Player player);

 private:
  void effectEnemies( Player enemy, int x, int y );
  int countLiberties( Player player, int x, int y );
  void clearMarks();
  void clearGroup( Player player, int x, int y );
};
