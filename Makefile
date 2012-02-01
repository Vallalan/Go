CC = g++
DEBUG = -g
WARN = -Wall -pedantic -ansi


GAME_H = Stone.h GoBoard.h
GAME_SRC = Stone.cpp GoBoard.cpp

CONSOLE_SRC = GoMain.cpp $(GAME_SRC)

GUI_H = myarea.h
GUI_SRC = main.cpp myarea.cpp

FULLGUI_H = $(GAME_H) $(GUI_H)
FULLGUI_SRC = $(GAME_SRC) $(GUI_SRC)


GoGui: $(FULLGUI_H) $(FULLGUI_SRC)
	$(CC) $(FULLGUI_SRC) -o $@ `pkg-config gtkmm-3.0 --cflags --libs`

GoConsole: $(GAME_H) $(CONSOLE_SRC)
	$(CC) $(CONSOLE_SRC) -o $@

clean:
	rm *.o Go *.*~ *~ 