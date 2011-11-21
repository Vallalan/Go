/*   Author: Alan Gieske
             Allen Hsia
     Project: Go
     
     Description: Assignment for programming languages course
     GoBoard builds the board and is responsible for
     executing each game step.
 */
#include "GoBoard.h"

//builds a 2d array of stone*
GoBoard::GoBoard( int width, int height ) 
{
  //set the board width and height
  this->width = width;
  this->height = height;
  emptyOwner = EMPTY;
  
  //make space for the grid
  grid = new Stone **[width];
  for( int x = 0; x<width; x++ ) 
    {
      grid[x] = new Stone *[height];
      for( int y = 0; y < height; y++ ) 
	{
	  grid[x][y] = new Stone( EMPTY, x, y );
	}
    }
}

GoBoard::~GoBoard() 
{
  for( int x = 0; x<width; x++ )
    {
    for( int y = 0; y<height; y++ ) 
      {
	delete grid[x][y]; //iterate through the board deleteing all stones
      }
    
    delete [] grid[x]; //delete each column
    }
  
  delete [] grid; //delete the grid
}

int GoBoard::step( Player player ) 
{
  Player currPlayer = player;
  Player Enemy;
  int x, y;
  Stone *currStone;
  Group playerGroup;
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
	std::cout << "Please enter the desired row, or enter -1 to pass " << std::endl;
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

  playerGroup.size = 0;
  playerGroup.liberties = 0;
  playerGroup.player = currPlayer;
  playerGroup.owner = (Player)EMPTY;
  playerGroup = getGroup( x, y, playerGroup );
  std::cout<<"Size: "<<playerGroup.size<<" Liberties: "<<playerGroup.liberties<<" Player: "<<playerGroup.player<<" Owner: "<<playerGroup.owner<<std::endl;
  //test if player killed themselves
  if( playerGroup.liberties == 0 )
    {
      clearGroup( currPlayer, x, y );
    }
  
  //clear all our Counted marks
  clearMarks();

  //tell the main loop that the player made a move
  return 1;  
}

/*
  Determines if a move is valid
  
  param: int x - the desired column
  param: int y - the desired row

  return: Bool - whether the move is valid
*/
bool GoBoard::isOnBoard( int x, int y )
{
  // check that move is inside the board space
  if( x >= 0 && x < width && y >= 0 && y < height )
    {
      return true;
    }
    
    return false;
}

/*
  Probes adjecent stones and determines if any neighbor groups
  no longer have liberties

  param enemy: the color of the opponents stones
  param x, y: coordinates of the newly placed stone

  return Bool, KOflag, returns true if a state of KO may be possible
  this flag, combined with the flag in the main step function, determines
  if there is in fact a KO state.
*/
bool GoBoard::effectEnemies( Player enemy, int x, int y ) 
{
  //test for enemies
  
  //flags to see if a state of KO may be possible
  int killSize = 0; //size of last gorup killed, used to test for KO
  int numKilled = 0; //groups killed, used to test for KO

  //create a group to be reused to test all enemies
  Group enemyGroup;
  enemyGroup.player = enemy;
  enemyGroup.owner = (Player)EMPTY;
  enemyGroup.size = 0;
  enemyGroup.liberties = 0;
  

  //array's to represent orthogonal neighbors
  int xnew[4] = { x-1, x, x+1, x };
  int ynew[4] = { y, y-1, y, y+1 };

  //iterate through each neighbor
  for( int i = 0; i < 4; i++ )
    {
      //make sure we don't go outside the board
      if( isOnBoard( xnew[i], ynew[i] ))
	{
	  
	  //see if the stone is an enemy and has NOT been counted yet
	  if( grid[xnew[i]][ynew[i]]->isPlayer( enemy ) && 
	      !grid[xnew[i]][ynew[i]]->isCounted() )
	    {
	      enemyGroup.size = 0;
	      enemyGroup.liberties = 0;
	      enemyGroup = getGroup( xnew[i], ynew[i], enemyGroup );
	      if( enemyGroup.liberties == 0 ) 
		{
		  //set the killSize when a group is killed
		  killSize = enemyGroup.size;
		  numKilled += 1;
		  //set all stones to empty
		  clearGroup( enemy, xnew[i], ynew[i] );
		}
	    }
	}
    }

  //if a single group of size 1 is killed then we need to return that a
  //KO may be possible depending on the size of the player group
  if( numKilled == 1 && killSize == 1)
    {
      return true;
    }
  return false;
}


void GoBoard::clearGroup( Player player, int x, int y ) 
{
  //clear the first node
  grid[x][y]->setPlayer( EMPTY );

  //array's for the orthogonal neighbors
  int xnew[4] = { x-1, x, x+1, x };
  int ynew[4] = { y, y-1, y, y+1 };

  //iterate through each neighbor
  for( int i = 0; i < 4; i++ )
    {

      //make sure we don't go outside the board
      if( xnew[i] >= 0 && xnew[i] < width && ynew[i] >= 0 && ynew[i] < height ) 
	{
	  //make sure the stone is of the correct player
	  if( grid[xnew[i]][ynew[i]]->getPlayer() == player )
	    {
	      //clear the stone and any stones of the same player attached to it
	      clearGroup( player, xnew[i], ynew[i] );
	    }
	}
    }
}

void GoBoard::clearMarks()
{

  //clear all the counted marks on the board
  for( int x = 0; x < width; x++ ) 
    {
      for( int y = 0; y < height; y++ )
	{
	  grid[x][y]->clearCounted();
	}
    }
}

Player GoBoard::scoreArea()
{
  int emptySize;
  int scoreBlack = 0;
  int scoreWhite = 0;
  for( int x = 0; x < width; x++ )
    {
      for( int y = 0; y < height; y++ )
	{
	  if( !grid[x][y]->isCounted() )
	    {
	      switch( grid[x][y]->getPlayer() )
		{
		case( BLACK ):

		  //find the size of the black group
		  scoreBlack += getSize( BLACK, x, y );
		  break;
		case( WHITE ):

		  //find the size of the white group
		  scoreWhite += getSize( WHITE, x, y );
		  break;
		case( EMPTY ):
		  emptySize = 0;

		  //find the size of the empty area
		  emptySize = scoreEmpty( x, y );
		  
		  //figure out the owner of the controlled territory
		  //add it to their score
		  switch( emptyOwner )
		    {
		    case( BLACK ):
		      scoreBlack += emptySize;
		      break;
		    case( WHITE ):
		      scoreWhite += emptySize;
		      break;
		    default:
		      break;
		    }
		  emptyOwner = EMPTY;
		  break;
		default:
		  break;
		}
	      //set the grid piece as counted
	      grid[x][y]->setCounted();
	    }
	}
    }

  //find the bigger number
  if( scoreWhite > scoreBlack )
    {
      return WHITE;
    }
  else if( scoreBlack > scoreWhite )
    {
      return BLACK;
    }
  else
    {
      return EMPTY;
    }
}

int GoBoard::scoreEmpty( int x, int y )
{
  //don't double count
  grid[x][y]->setCounted();
  int size = 0;

  //array's for the orthogonal neighbors
  int xnew[4] = { x-1, x, x+1, x };
  int ynew[4] = { y, y-1, y, y+1 };

  //iterate through each neighbor
  for( int i = 0; i < 4; i++ )
    {

      //make sure we don't go outside the board
      if( xnew[i] >= 0 && xnew[i] < width && ynew[i] >= 0 && ynew[i] < height ) 
	{
	  //Don't double count
	  if( !grid[xnew[i]][ynew[i]]->isCounted() )
	    {
	      //if neighbor is empty keep counting
	      if( grid[xnew[i]][ynew[i]]->getPlayer() == EMPTY )
		{
		  size += scoreEmpty( xnew[i], ynew[i] );
		}
	      //if we haven't run into a player owned piece yet, or we run into
	      //the same players piece, that player still owns the group
	      else if( grid[xnew[i]][ynew[i]]->getPlayer() == emptyOwner || emptyOwner == EMPTY )
		{
		  emptyOwner = (Player)grid[xnew[i]][ynew[i]]->getPlayer();
		}
	      else
		{
		  //set the empty group to dead,
		  //no one gets the points
		  emptyOwner = DOMI;
		}
	    }
	}
    }
  //return the size plus one for the stone we're on
  return size + 1;
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
	
/*
  Counts the all neighbors of a specific player around the
  provided stone.

  param target: the player type we are counting
  param x and y: the grid coordinates of the center stone

  return count: the number of neighbors of target color
*/
int GoBoard::countNeighbors( Player target, int x, int y )
{
  int count = 0;
  int xnew[4] = { x-1, x, x+1, x };
  int ynew[4] = { y, y-1, y, y+1 };

  for( int i = 0; i<4; i++ )
    {
      if( isOnBoard( xnew[i], ynew[i] ))
	{
	  if( grid[xnew[i]][ynew[i]]->getPlayer() == target)
	    {
	      count += 1;
	    }
	}
    }
  return count;
}

Group GoBoard::getGroup( int x, int y, Group currGroup )
{
  //coordinates of all neighbor stones
  int xnew[] = { x-1, x, x+1, x };
  int ynew[] = { y, y-1, y, y+1 };

  //set as counted to prevent double count
  grid[x][y]->setCounted();

  //iterate through every neighbor
  for( int i = 0; i < 4; i++ )
    {
      //verify we're on the board
      if( isOnBoard( xnew[i], ynew[i] ))
	{
	  //verify we have the same player
	  if( grid[xnew[i]][ynew[i]]->isPlayer(currGroup.player))
	    {
	      //check if already counted
	      if( !grid[xnew[i]][ynew[i]]->isCounted())
		{
		  currGroup = getGroup( xnew[i], ynew[i], currGroup );
		}
	    }
	  else if( currGroup.owner == EMPTY || 
		   grid[xnew[i]][ynew[i]]->isPlayer(currGroup.owner))
	    {
	      currGroup.owner = (Player)grid[xnew[i]][ynew[i]]->getPlayer();
	    }
	  else
	    {
	      currGroup.owner = (Player)DOMI;
	    }
	}
    }

  currGroup.size += 1;
  currGroup.liberties += countNeighbors( EMPTY, x, y );
  std::cout<<"x: "<< x <<" y: "<< y << " lib: "<<countNeighbors( EMPTY, x, y )<<std::endl;
  return currGroup;
}
  
