/*   Author: Alan Gieske
             Allen Hsia
     Project: Go
     
     Description: Assignment for programming languages course
     GoBoard builds the board and is responsible for
     executing each game step.
 */
#include "GoBoard.h"

//static variables to hold the count of captured stones
static int blackPrisoners;
static int whitePrisoners;

//hold the position of a KO if it exists
int koPosX;
int koPosY;

//builds a 2d array of stone*
GoBoard::GoBoard( int width, int height ) 
{
  //set the board width and height
  this->width = width;
  this->height = height;
  
  blackPrisoners = 0;
  whitePrisoners = 0;

  //set to -1 when there is no KO on the board
  koPosX = -1;
  koPosY = -1;
 
  
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
 

int GoBoard::step( Player player, int x, int y ) 
{
  Player currPlayer = player;
  Player Enemy;
  Stone *currStone;
  bool KOflag1 = false;
  Group playerGroup;
 
  std::string playerName;

  if( currPlayer == BLACK ) 
    {
      Enemy = WHITE;
    } 
  else 
    {
      Enemy = BLACK;
    }
 
  currStone = grid[x][y];
  //set piece
  currStone->setPlayer( currPlayer ); 

  //test and possibly kill enemies
  KOflag1 = effectEnemies( Enemy, x, y );

  //set the attributes of the group for the current player
  playerGroup.size = 0;
  playerGroup.liberties = 0;
  playerGroup.player = currPlayer;
  playerGroup.owner = (Player)EMPTY;
  playerGroup = getGroup( x, y, playerGroup );

  //Debug message to see if the liberties, size, and owner are counted correctly
  std::cout<<"Size: "<<playerGroup.size<<" Liberties: "<<playerGroup.liberties<<" Player: "<<playerGroup.player<<" Owner: "<<playerGroup.owner<<std::endl;


  //test if player killed themselves
  if( playerGroup.liberties == 0 )
    {
      clearGroup( currPlayer, x, y );
    }

  //Determine if there is not a KO
  if( playerGroup.liberties != 1 || playerGroup.size != 1 || !KOflag1 )
    {
      //The KO positions are set when an enemy is killed,
      //if a KO is not possible then the positions need to be reset before
      //the step finishes
      koPosX = -1;
      koPosY = -1;
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
 
  
  //flags to see if a state of KO may be possible
  int killSize = 0; //size of last gorup killed, used to test for KO, and scoring
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
		  
		  //sets the KO position temporarily
		  //will be cleared in the step function if
		  //a ko is not present
		  koPosX = xnew[i];
		  koPosY = ynew[i];
		  
		  switch( enemyGroup.player )
		    {
		    case(WHITE):
		      {
			blackPrisoners += killSize;
			break;
		      }
		    case(BLACK):
		      {
			whitePrisoners += killSize;
			break;
		      }
		    default:
		      {
			break;
		      }
		    }
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

/*
  Clears a group of stones from the board

  param player: color of the group
  param x, y: coordinates of a stone within the group
*/
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
      if( isOnBoard( xnew[i],ynew[i] ))
	{
	  //make sure the stone is of the correct player
	  if( grid[xnew[i]][ynew[i]]->isPlayer( player ))
	    {
	      //clear the stone and any stones of the same player attached to it
	      clearGroup( player, xnew[i], ynew[i] );
	    }
	}
    }
}

/*
  clears all of our counted marks on the board
*/
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

/*
  Scores the board according to chinese Area scoring rules
  -each stone on the board is a point for it's owning player
  -each empty group that is completely surrounded by just 1 player's
   stones and board edges has its size scored for that player.

   returns the player who has the higher score
*/
Player GoBoard::scoreArea()
{
  Group currGroup;
  int scoreBlack = 0;
  int scoreWhite = 0;
  for( int x = 0; x < width; x++ )
    {
      for( int y = 0; y < height; y++ )
	{
	  currGroup.size = 0;
	  currGroup.liberties = 0;
	  currGroup.owner = EMPTY;
	  if( !grid[x][y]->isCounted() )
	    {
	      switch( grid[x][y]->getPlayer() )
		{
		case( BLACK ):
		  currGroup.player = (Player)BLACK;
		  currGroup = getGroup( x, y, currGroup );
		  scoreBlack += currGroup.size;
		  break;
		case( WHITE ):
		  currGroup.player = (Player)WHITE;
		  currGroup = getGroup( x, y, currGroup );
		  scoreWhite += currGroup.size;
		  break;
		case( EMPTY ):
		  currGroup.player = (Player)EMPTY;
		  currGroup.owner = (Player)EMPTY;
		  currGroup = getGroup( x, y, currGroup);
		  //find the size of the empty area
		  
		  
		  //figure out the owner of the controlled territory
		  //add it to their score
		  switch( currGroup.owner )
		    {
		    case( BLACK ):
		      scoreBlack += currGroup.size;
		      break;
		    case( WHITE ):
		      scoreWhite += currGroup.size;
		      break;
		    default:
		      break;
		    }		  
		  break;
		default:
		  break;
		}
	    }
	}
    }

  std::cout<<"Black's Score "<<scoreBlack<<std::endl;
  std::cout<<"White's Score "<<scoreWhite<<std::endl;

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

/*
  Scores the board according to Japanese/Korean Territory scoring rules
  -each stone captured during the course of the game is added to the
   capturing players score
  -each empty group that is completely surrounded by just 1 player's
   stones and board edges has its size scored for that player.
   
   return the player who has the higher score
*/
Player GoBoard::scoreTerritory()
{
  Group currGroup;
  int scoreBlack;
  int scoreWhite;
  
  std::cout<<"Black's captured stones "<<blackPrisoners<<std::endl;
  std::cout<<"White's captured stones "<<whitePrisoners<<std::endl;
  //add each players captured prisoners to their score
  scoreBlack = blackPrisoners;
  scoreWhite = whitePrisoners;

  //iterate through the board piece by piece
  for( int x = 0; x < width; x++ )
    {
      for( int y = 0; y < height; y++ )
	{

	  //reset the group attributes
	  currGroup.size = 0;
	  currGroup.liberties = 0;
	  currGroup.owner = (Player)EMPTY;
	  currGroup.player = (Player)EMPTY;
	  
	  if( grid[x][y]->isPlayer( EMPTY ) && !grid[x][y]->isCounted())
	    {
	      //get attributes of the empty territory
	      currGroup = getGroup( x, y, currGroup );
	      std::cout<<"Size: "<<currGroup.size<<" owner: "<<currGroup.owner<<std::endl;

	      //determine 'Ownership' of the empty territory and add the size to that player score
	      switch( currGroup.owner )
		{
		case(BLACK):
		  {
		    scoreBlack += currGroup.size;
		    break;
		  }
		case(WHITE):
		  {
		    scoreWhite += currGroup.size;
		    break;
		  }
		default:
		  { break; }
		}
	    }
	}
    }
  
  std::cout<<"Black's Score "<<scoreBlack<<std::endl;
  std::cout<<"White's Score "<<scoreWhite<<std::endl;
  
  //Determine winner and return that player
  if( scoreBlack > scoreWhite )
    {
      return BLACK;
    }
  else if ( scoreWhite > scoreBlack )
    {
      return WHITE;
    }
  else
    {
      return EMPTY;
    }

}

/*
  iterates through each stone on the board printing out
  it's assigned character(its player)
*/
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
	  if( grid[xnew[i]][ynew[i]]->isPlayer(target))
	    {
	      count += 1;
	    }
	}
    }
  return count;
}

/*
  core function: recursively traces through every stone that is
  orthogonally connected starting at stone x,y. as it traverses it counts
  and updates the attributes Size, Liberties, and Owner, and returns them
  wrapped in a Group structure

  param x,y: coordinates of a stone within a group
  param currGroup: a copy of a Group object that holds all attributes
  
  returns currGroup after traversing all stones in the group
*/
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
  //std::cout<<"x: "<< x <<" y: "<< y << " lib: "<<countNeighbors( EMPTY, x, y )<<std::endl;
  return currGroup;
}

/*
  Wrapper function to allow access to the Stone's isPlayer function
  
  param x,y: coordinates of the desired stone
  param target: desired player

  return true if the target is the same as the stones player, else false
*/
bool GoBoard::isPlayer( Player target, int x, int y )
{
  return grid[x][y]->isPlayer( target );
}
 
/*
  determines if a desired position is restricted due to KO

  KO- a state where by playing a stone you return the board to
      the position it was in the previous turn, this is disallowed
      by standard rules as it can create an infinite loop of play
      note that the restriction only stands for a single turn as it
      forces the player to make a different move and create a different
      board.

*/
bool GoBoard::isKo( int x, int y )
{
  if( x == koPosX && y == koPosY )
    {
      return true;
    }
  return false;
}

/*
  Determines if the position is available to have a stone placed in it

  param x,y: integer coordinates of the stone to place

  return bool: whether a stone can be placed there
*/
bool GoBoard::isValidMove( int x, int y)
{
  if( isOnBoard( x, y ) && isPlayer( EMPTY, x, y ) && !isKo( x, y ))
    {
      return true;
    }
  return false;
}


