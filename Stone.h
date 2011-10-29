/*   Author: Alan Gieske
     Project: Go
     
     Description: Assignment for programming languages course
     Stone class header
 */

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
  int Counted; //1 means counted 0 means not counted

 public:
  Stone() {Counted = 0;};
  ~Stone() {};
  
  Stone(Player player, int x, int y);

  void clearCounted();
  void setCounted();
  int isCounted();
  void setY(int y);
  void setX(int x);
  void setPlayer(Player player);
  char getPlayer();
  int getX();
  int getY();
};
