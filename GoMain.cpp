#include "GoBoard.h"
#include<iostream>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

int main() {
  GoBoard  board(BOARD_WIDTH, BOARD_HEIGHT);
  board.printboard();
  while(1) {
    int bMove, wMove;

    
    bMove = board.step(BLACK);
    board.printboard();
    wMove = board.step(WHITE);
    board.printboard();
    

    //if both players have passed, skip turn
    if( bMove == -1 && wMove == -1 ) {
      break;
    }
  }

  //score and declare winner here

  return 0;
}
