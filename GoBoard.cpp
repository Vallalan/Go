

#include "GoBoard.h"

//function to get a stone pointer
Stone *makeStone( Player player, int x, int y ) 
{
  return new Stone( player, x, y );
}

GoBoard::GoBoard( int width, int height ) 
{
  //set the board width and height
  this->width = width;
  this->height = height;
  
  //make space for the grid
  grid = new Stone **[width];
  for( int x = 0; x<width; x++ ) 
    {
      grid[x] = new Stone *[height];
      for( int y = 0; y < height; y++ ) 
	{
	  grid[x][y] = makeStone( EMPTY, x, y );
	}
    }
}

GoBoard::~GoBoard() 
{
  for( int x = 0; x<width; x++ )
    {
    for( int y = 0; y<height; y++ ) 
      {
	delete grid[x][y];             //iterate through the board deleteing all stones
      }
    
    delete [] grid[x];              //delete each column
    }
  
  delete [] grid;                  //delete the grid
}

int GoBoard::step( Player player ) 
{
  Player currPlayer = player;
  Player Enemy;
  int x, y;
  Stone *currStone;
  std::string playerName;

  //set player name as string
  if( currPlayer == BLACK ) 
    {
      playerName = "Black";
    }
  else
    {
      playerName = "White";
    }
  
  std::cout << "It is now " << playerName << "\'s turn" << std::endl;
  if( currPlayer == BLACK ) 
    {
      Enemy = WHITE;
    } 
  else 
    {
      Enemy = BLACK;
    }

  //get move from user
  //ugly, ugly, code
  while( 1 )
    {
    while( 1 ) 
      {
	std::cout << "Please enter the desired column, or enter -1 to pass ";
	std::cin >> x;
	if( x == -1 )
	  {
	    return -1;
	  }
      if( x < 0 || x >= width ) 
	{
	  std::cout << "Please enter a valid column " << std::endl;
	}
      else 
	{
	  break;
	}
      }
    while( 1 ) 
      {
	std::cout << "Please enter the desired row, or enter -1 to pass ";
	std::cin >> y;
      if( y == -1 ) 
	{
	  return -1;
	}
      if( y < 0 || y >= height ) 
	{
	  std::cout << "Please enter a valid row " << std::endl;
	} 
      else 
	{
	  break;
	}
      }
    if( grid[x][y]->getPlayer() == EMPTY )
      {
	currStone = grid[x][y];
	break;
      }
    else
      {
	std::cout << "Please enter Col/Row of an empty space." << std::endl;
      }
    }


  
  //set piece
  currStone->setPlayer( currPlayer );

  //test and possibly kill enemies
  effectEnemies( Enemy, x, y );

  //test if player killed themselves
  if( countLiberties( currPlayer, x, y ) == 0 )
    {
      clearGroup( currPlayer, x, y );
    }
  
  clearMarks();
  return 1;  
}

void GoBoard::effectEnemies( Player enemy, int x, int y ) 
{
  //test for enemies

  //Check West
  if( x-1 >= 0 ) 
    {
      if( grid[x-1][y]->getPlayer() == enemy ) 
	{
	  if( countLiberties( enemy, x-1, y ) == 0 ) 
	    {
	      clearGroup( enemy, x-1, y );
	    }
	}
    }
  
  //Check North
  if( y-1 >= 0 ) 
    {
      if( grid[x][y-1]->getPlayer() == enemy && grid[x][y-1]->isCounted() == 0 ) 
	{
	  if( countLiberties( enemy, x, y-1 ) == 0 ) 
	    {
	      clearGroup( enemy, x, y-1 );
	    }
	}
    }

  //Check East
  if( x+1 < width ) 
    {
      if( grid[x+1][y]->getPlayer() == enemy && grid[x+1][y]->isCounted() == 0 ) 
	{
	  if( countLiberties( enemy, x+1, y ) == 0 ) 
	    {
	      clearGroup( enemy, x+1, y );
	    }
	}
    }

  //Check South
  if( y+1 < height ) 
    {
      if( grid[x][y+1]->getPlayer() == enemy && grid[x][y+1]->isCounted() == 0 ) 
	{
	  if( countLiberties( enemy, x, y+1 ) == 0 ) 
	    {
	      clearGroup( enemy, x, y+1 );
	    }
	}
    }
  
}

int GoBoard::countLiberties( Player player, int x, int y ) 
{
  int Liberties = 0;
  
  //mark that a stone has already been counted
  grid[x][y]->setCounted();

  //count EAST
  if( x-1 >= 0 )
    {
      if( grid[x-1][y]->getPlayer() == player )
	{
	  if( grid[x-1][y]->isCounted() == 0 )
	    {
	      Liberties += countLiberties( player, x-1, y );
	    } 
	}
      else if( grid[x-1][y]->getPlayer() == EMPTY )
	{
	  return 1;
	}
      
    }
  
  //Repeat for West
  if( x+1 < width )
    {
      if( grid[x+1][y]->getPlayer() == player )
	{
	  if( grid[x+1][y]->isCounted() == 0 )
	    {
	      Liberties += countLiberties( player, x+1, y );
	    }
	}
      else if( grid[x+1][y]->getPlayer() == EMPTY )
	{
	  return 1;
	}
	
	
    }

  if( y-1 >= 0 )
    {
      if( grid[x][y-1]->getPlayer() == player )
	{
	  if( grid[x][y-1]->isCounted() == 0 )
	    {
	      Liberties += countLiberties( player, x, y-1 );
	    }
	}
      else if( grid[x][y-1]->getPlayer() == EMPTY )
	{
	  return 1;
	}
    }

  if( y+1 < height )
    {
      
      if(grid[x][y+1]->getPlayer() == player )
	{
	  if( grid[x][y+1]->isCounted() == 0 )
	    {
	      Liberties += countLiberties( player, x, y+1 );
	    }
	}
      else if( grid[x][y+1]->getPlayer() == EMPTY )
	{
	  return 1;
	}
    }
 
  
  //if Liberties is 0 here, the group is dead
  return Liberties;
}

void GoBoard::clearGroup( Player player, int x, int y ) 
{
  grid[x][y]->setPlayer( EMPTY );
  if( x-1 >= 0 ) 
    {
      if( grid[x-1][y]->getPlayer() == player )
	{
	  clearGroup( player, x-1, y );
	}
    }

  if( y-1 >= 0 ) 
    {
      if( grid[x][y-1]->getPlayer() == player )
	{
	  clearGroup( player, x, y-1 );
	}
    }

  if( x+1 < width ) 
    {
      if( grid[x+1][y]->getPlayer() == player )
	{
	  clearGroup( player, x+1, y );
	}
    }

  if( y+1 < height ) 
    {
      if( grid[x][y+1]->getPlayer() == player )
	{
	  clearGroup( player, x, y+1 );
	}
    }
}

void GoBoard::clearMarks()
{
  for( int x = 0; x < width; x++ ) 
    {
      for( int y = 0; y < height; y++ )
	{
	  grid[x][y]->clearCounted();
	}
    }
}

void GoBoard::printboard() 
{
  std::cout << "  0 1 2 3 4 5 6 7 8 9" << std::endl;
  for( int y = 0; y<height; y++ ) 
    {
      std::cout << y;

      for( int x = 0; x<width; x++ ) 
	{
	  std::cout << " " << grid[x][y]->getPlayer();
	}

      std::cout << std::endl;
    }
}
	  
  
