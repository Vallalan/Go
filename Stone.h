/*   Author: Alan Gieske
             (Allen Hsia)
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
  bool Counted; //1 means counted 0 means not counted

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
