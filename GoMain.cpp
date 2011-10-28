/*    Authort: Alan Gieske
      Project: Go

      Description: Assignment for programming languages class
      Main game driver
*/

#include "GoBoard.h"
#include<iostream>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

int main() {
  GoBoard  board(BOARD_WIDTH, BOARD_HEIGHT);
  int bMove, wMove = 0; //values to keep track of who has passed

  //print initial blank board
  board.printboard();
  
  //Main game loop
  while(1) {
    

    //perform black's move
    bMove = board.step(BLACK);
    board.printboard();

    //check for 2 passes in a row
    if( bMove == -1 && wMove == -1 ) {
      break;
    }
    
    //perform white's move
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
