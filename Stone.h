/*   Author: Alan Gieske
             (Allen Hsia)
     Project: Go
     
     Description: 
     Stone class header
 */

#ifndef STONE_H_
#define STONE_H_

typedef enum Player_t {
  BLACK = 'B',
  WHITE = 'W',
  EMPTY = '.',
  DOMI = 'D'
} Player;

class Stone {

 private:
  int x;
  int y;
  Player player;
  bool Counted; 

 public:
  Stone() {Counted = 0;};
  ~Stone() {};
  
  Stone(Player player, int x, int y);

  //control for the counted flag
  void clearCounted();
  void setCounted();
  bool isCounted();
  
  //get/set and boolean test for the Player
  void setPlayer(Player player);
  char getPlayer();
  bool isPlayer(Player target);
  
  //Getter/setters for coordinates
  void setY(int y);
  void setX(int x);
  int getX();
  int getY();
};

#endif //STONE_H_
