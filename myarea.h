/*
  Author: Allen Hsia
          (Alan Gieske)

          Custom Drawing area widget to handle the game board and user input
*/

#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H


#include "GoBoard.h"
#include <gtkmm/drawingarea.h>
#define BOARD_HEIGHT 10;
#define BOARD_WIDTH 10;
using namespace Gtk;
class MyArea : public Gtk::DrawingArea
{
public:
  int passCount;
  int playerTurn;
  GoBoard* board;
  MyArea();
  virtual ~MyArea();
  void PassTurn();
  int getScoreBlack();
  int getScoreWhite();
  
protected:
  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  virtual bool on_click(GdkEventButton* event);
};

#endif // GTKMM_EXAMPLE_MYAREA_H
