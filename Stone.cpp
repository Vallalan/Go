
#include "Stone.h"

Stone::Stone(char Player, int x, int y) {
  this->y = y;
  this->x = x;
  this->Player = Player;
  Counted = 0;
}
void Stone::setY(int y) {
  this->y = y;
}

void Stone::setX(int x) {
  this->x = x;
}

void Stone::setPlayer(char Player) {
  this->Player = Player;
}

void Stone::setCounted() {
  Counted = 1;
}

void Stone::clearCounted() {
  Counted = 0;
}
  

char Stone::getPlayer() {
  return Player;
}

int Stone::getX() {
  return x;
}

int Stone::getY() {
  return y;
}

int Stone::isCounted() {
  return Counted;
}


