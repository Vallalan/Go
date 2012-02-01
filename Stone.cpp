/*   Author: Alan Gieske
             (Allen Hsia)
     Project: Go
     
     Description: 
     Holds and allows access/modification to the data for individual game pieces
 */

#include "Stone.h"

Stone::Stone( Player player, int x, int y ) 
{
  this->y = y;
  this->x = x;
  this->player = player;
  Counted = 0;
}

//group of get/set methods
void Stone::setY(int y) 
{
  this->y = y;
}

void Stone::setX(int x) 
{
  this->x = x;
}

void Stone::setPlayer( Player player ) 
{
  this->player = player;
}
  
char Stone::getPlayer() 
{
  return player;
}

bool Stone::isPlayer( Player target )
{
  if( target == player )
    {
      return true;
    }
  return false;
}

int Stone::getX() 
{
  return x;
}

int Stone::getY() 
{
  return y;
}

//set up to only allow boolean access to Counted variable
void Stone::setCounted() 
{
  Counted = true;
}

void Stone::clearCounted() 
{
  Counted = false;
}

bool Stone::isCounted() 
{
  return Counted;
}


