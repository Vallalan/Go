
#define Empty = '.'

class Stone {

 private:
  int x;
  int y;
  char Player;
  int Counted; //1 means counted 0 means not counted

 public:
  Stone() {Counted = 0;};
  ~Stone() {};
  
  Stone(char Player, int x, int y);

  void clearCounted();
  void setCounted();
  int isCounted();
  void setY(int y);
  void setX(int x);
  void setPlayer(char Player);
  char getPlayer();
  int getX();
  int getY();
};
