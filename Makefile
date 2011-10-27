

CC = g++
DEBUG = -g
WARN = -Wall -pedantic -ansi
STUFLAGS = $(DEBUG) $(WARN)

PARTS_H = Stone.h
PARTS_SRC = Stone.cpp

GAME_H = $(PARTS_H) GoBoard.h
GAME_SRC = Stone.cpp GoBoard.cpp GoMain.cpp

GO: $(GAME_H) $(GAME_SRC)
	$(CC) $(STUFLAGS) -o $@ $(GAME_SRC)

clean:
	rm -f *.o *~ GO