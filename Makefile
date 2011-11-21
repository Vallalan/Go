#Author Alan Gieske & Allen Hsia

CC = g++
DEBUG = -g
WARN = -Wall -pedantic -ansi
STUFLAGS = $(DEBUG) $(WARN)

PARTS_H = Stone.h
PARTS_SRC = Stone.cpp

GAME_H = $(PARTS_H) GoBoard.h
GAME_SRC = Stone.cpp GoBoard.cpp GoMain.cpp

GUI_SRC = gtkTest.cpp

Go: $(GAME_H) $(GAME_SRC)
	$(CC) $(STUFLAGS) -o $@ $(GAME_SRC)
gui: $(GUI_SRC)
	$(CC) $(GUI_SRC) -o $@ `pkg-config gtkmm-3.0 --cflags --libs`

clean:
	rm -f *.o *~ Go gui