/*
  Author: Allen Hsia
          (Alan Gieske)

	  Main Gui window and controller for Go Game.
*/

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include "myarea.h"
#include <gtkmm.h>
#include <iostream>
using namespace Gtk;

//global pointers for widgets that need to be accessed by the pass button event
MyArea* area;
Label* score1;
Label* score2;

void Pass()
{

  
  std::string scoreBlack = "";
  std::string scoreWhite = "";
  std::stringstream convert;
  std::stringstream convert2;

  //tell the area widget to calculate a pass
  area->PassTurn();
  //if there are 2 passes in a row, get the scores from the area widget
  //and update appropriately
  if( area->passCount >= 2 ){
    convert << area->getScoreBlack();
    scoreBlack = convert.str();
    convert2 << area->getScoreWhite();
    scoreWhite = convert2.str();

    score1->set_text(scoreBlack);
    score2->set_text(scoreWhite);
    
  }
  if(area->playerTurn == 1){
    std::cout<<"Black has passed, it is now Red's turn"<<std::endl;
  }else{
    std::cout<<"Red has passed, it is now Black's turn"<<std::endl;
  }
}

int main(int argc, char** argv)
{
   Gtk::Main kit(argc, argv);
   Gtk::Window win;
   win.set_title("DrawingArea");
   area = new MyArea();
   score1 = new Label("");
   score2 = new Label("");
   Label turn("Player");
   Label red("Score Red:");
   Label black("Score Black:");
   VBox vbox;//adds the text for score
   VBox vbox1;
   HBox hbox;

   Button pass("Pass!");
   pass.signal_clicked().connect(sigc::ptr_fun(&Pass));

   //formatting the score
   vbox.pack_start(black,false,false,5);
   vbox.pack_start(*score1,false,false,5);
   vbox.pack_start(red,false,false,5);
   vbox.pack_start(*score2,false,false,5);
   vbox.pack_start(pass, false, false,5);
   //formatting the drawing area and the player's turn display

   vbox1.pack_start(*area, true,  true , 5);
   //vbox1.pack_start(turn, false, false, 5);
   //add them two toegther
   hbox.pack_start(vbox1, true, true, 5);
   hbox.pack_start(vbox, false, false, 5);
   win.add(hbox);

   win.resize(420,360);
   win.show_all();
   //Profit!
   Gtk::Main::run(win);

   return 0;
}
