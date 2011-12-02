/*    Authort: Alan Gieske
               (Allen Hsia)
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
  
  int x, y;
  //Main game loop
  while(1) {

    //get move from user
    while(1) {
      //reset coordinates to a completely invalid value
      x = -2;
      y = -2;
      std::cout<<"It is now Black's turn, \nplease enter desired move - 'column' 'row' or enter -1 for either one to pass."<<std::endl;

      //get coordinate input from user
      std::cin>>x>>y;
      
      //determine if it's a valid move, or if the user has passed, exit the loop
      if( board.isValidMove( x, y ) || x == -1 || y == -1) {
	break;
      }
      else {
	std::cout<<"Please enter a valid space"<<std::endl;
      }
    }
    
    //
    if( x!=-1 && y!=-1 ) {
      //perform black's move
      bMove = board.step(BLACK, x, y);
      //reprint the board
      board.printboard();
    } else {
      bMove = -1;
    }
    //check for 2 passes in a row
    if( bMove == -1 && wMove == -1 ) {
      break;
    }

    //get move from user
    while(1) {
      //reset coordinates to a completely invalid value
      x = -2;
      y = -2;
      std::cout<<"It is now Whites's turn, \nplease enter desired move - 'column' 'row' or enter -1 for either one to pass."<<std::endl;

      //Take the coordinates from user
      std::cin>>x>>y;
      

      //determine if it is a valid move, or the user has passed
      if( board.isValidMove( x, y ) || x == -1 || y == -1) {
	break;
      }
      else {
	std::cout<<"Please enter a valid space"<<std::endl;
      }
    }
    if( x != -1 && y != -1 ){
      //perform white's move
      wMove = board.step(WHITE, x, y);
      board.printboard();
    } else {
      wMove = -1;
    }

    //if both players have passed, skip turn
    if( bMove == -1 && wMove == -1 ) {
      break;
    }
  }

  //determine the winner
  Player winner = board.scoreTerritory();

  switch( winner )
    {
    case( BLACK ):
      std::cout << "The winner is Black!!!" << std::endl;
      break;
    case( WHITE ):
      std::cout << "The winner is White!!!" << std::endl;
      break;
    default:
      std::cout << "The game is a draw!!!" << std::endl;
      break;
    }

  return 0;
}
