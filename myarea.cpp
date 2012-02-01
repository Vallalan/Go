/*
  Author: Allen Hsia
          (Alan Gieske)

	  Custom Drawing Area widget to display the board and take in user input
*/

#include <cairomm/context.h>
#include "myarea.h"
#include <iostream>
#include <math.h>





MyArea::MyArea(){
  //construct a new board
  board = new GoBoard(10, 10);
  //set the player turn to Black
  playerTurn = -1;
  //no passes yet
  passCount = 0;
  
  std::cout<<"It is now Black's Turn"<<std::endl;
  //connect the on click signal
  add_events(Gdk::BUTTON_PRESS_MASK);
  this->signal_button_press_event().connect(sigc::mem_fun(*this,&MyArea::on_click));
}

MyArea::~MyArea(){}

void MyArea::PassTurn(){
  //increment the player turn
  playerTurn = playerTurn * -1;
  passCount += 1;
  //if there are 2 passes, the board needs to be scored
  if(passCount >= 2){
    board->scoreArea();
    std::cout<<"There have been 2 passes, the board will now be scored"<<std::endl;
  }
  //redraw the board to show the new player color
  queue_draw();
}

//access to the current score(will be zero if the board has not been scored)
int MyArea::getScoreBlack(){
  return board->getScoreBlack();
}

//access to the current score(will be zero if the board has not been scored)
int MyArea::getScoreWhite(){
  return board->getScoreWhite();
}


/*
  gets the pixel coordinates of the user click shifts and divides to
  extract coordinates from 0 to 9 which it then sends to board to simulate a
  game step and update the board.
*/
bool MyArea::on_click(GdkEventButton* event)
{
  //get the mouse coordinates
  int mouse_x, mouse_y;
  mouse_x = event->x;
  mouse_y = event->y;

  //transform them to integer coordinates
  mouse_x = (mouse_x-10)/30;
  mouse_y = (mouse_y-10)/30;

  
  
  //ask the board if the coordinates are a valid move
  if(board->isValidMove( mouse_x, mouse_y ))
    {
      //determine whose turn it is and tell the board to make the move
      if( playerTurn == -1)
	{
	  //reset the pass count
	  passCount = 0;
	  board->step( BLACK, mouse_x, mouse_y );
	} 
      else 
	{
	  //reset the pass count
	  passCount = 0;
	  board->step( WHITE, mouse_x, mouse_y );
	}
      
      //increment the turn
      playerTurn = playerTurn*-1;
      //tell the board to redraw
      queue_draw();
      
      //confirm in the console that the move was made
      std::cout<<"You placed a stone at: "<<mouse_x<<" "<<mouse_y<<std::endl;
      
      //inform the next player that it is their turn
      if( playerTurn == 1) {
	std::cout<<"It is now Red's Turn"<<std::endl;
      }else{
	std::cout<<"It is now Black's Turn"<<std::endl;
      }
    }
  else
    {
      //display a message in the console to show the player their move
      //is invalid
      std::cout<<"Please select a valid space or pass."<<std::endl;
    }
  
  

  
      
  return true;
}

//redraws the board
bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  cr->set_line_width(1.0);
  
  // draw red or black lines depending on a static variable
  if( playerTurn == -1) {
    cr->set_source_rgb(0.0, 0.0, 0.0);
  }else{
    cr->set_source_rgb(255.0, 0.0, 0.0);
  }

  //iterate through the board
  for( int x = 0; x < 10; x++ )
    {
      for( int y = 0; y < 10; y++ )
	{
	  //save the context
	  cr->save();
	  
	  //determine the player of the stone, and color accordingly
	  switch(board->getPlayer( x, y ))
	    {
	    case('B'):
	      {
		cr->set_source_rgba( 0.0, 0.0, 0.0, 0.8 );
		break;
	      }
	    case('W'):
	      {
		cr->set_source_rgba( 255.0, 0.0, 0.0, 0.8 );
		break;
	      }
	    default:
	      {
		cr->set_source_rgba( 255.0, 255.0, 255.0, 0.0 );
		break;
	      }
	    }

	  //draw the circle and fill it.
	  cr->arc( x*30.0+25.0, y*30.0+25.0, 15.0, 0.0, 2.0 * M_PI );
	  cr->fill_preserve();
	  
	  //restore the context
	  cr->restore();
	  cr->stroke();
	  
	}
    }
  
  //vertical lines
  cr->move_to(10,10);
  cr->line_to(10,310);
  cr->stroke();
  cr->move_to(40,10);
  cr->line_to(40,310);
  cr->stroke();
  cr->move_to(70,10);
  cr->line_to(70,310);
  cr->stroke();
  cr->move_to(100,10);
  cr->line_to(100,310);
  cr->stroke();
  cr->move_to(130,10);
  cr->line_to(130,310);
  cr->stroke();
  cr->move_to(160,10);
  cr->line_to(160,310);
  cr->stroke();
  cr->move_to(190,10);
  cr->line_to(190,310);
  cr->stroke();
  cr->move_to(220,10);
  cr->line_to(220,310);
  cr->stroke();
  cr->move_to(250,10);
  cr->line_to(250,310);
  cr->stroke();
  cr->move_to(280,10);
  cr->line_to(280,310);
  cr->stroke();
  cr->move_to(310,10);
  cr->line_to(310,310);
  cr->stroke();

  //horizontal lines
  cr->move_to(10,10);
  cr->line_to(310,10);
  cr->stroke();
  cr->move_to(10,40);
  cr->line_to(310,40);
  cr->stroke();
  cr->move_to(10,70);
  cr->line_to(310,70);
  cr->stroke();
  cr->move_to(10,100);
  cr->line_to(310,100);
  cr->stroke();
  cr->move_to(10,130);
  cr->line_to(310,130);
  cr->stroke();
  cr->move_to(10,160);
  cr->line_to(310,160);
  cr->stroke();
  cr->move_to(10,190);
  cr->line_to(310,190);
  cr->stroke();
  cr->move_to(10,220);
  cr->line_to(310,220);
  cr->stroke();
  cr->move_to(10,250);
  cr->line_to(310,250);
  cr->stroke();
  cr->move_to(10,280);
  cr->line_to(310,280);
  cr->stroke();
  cr->move_to(10,310);
  cr->line_to(310,310);
  cr->stroke();
  return true;
}
