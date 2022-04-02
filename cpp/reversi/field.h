#ifndef _FIELD_HH
#define _FIELD_HH

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "window.h"
#include "text.h"
#include "image.h"
#include "chip.h"

#define FIELD_ROWS 8
#define FIELD_COLS 8
#define FIELD_SQUARE_SIDE 80


class Field: public Window {
public:
  enum CellState { kBusyCell = -1, kEmptyCell = 0, kAllowedCell = 1 };
    
  Field(Chip::ColorType turn = Chip::kNo);
  ~Field();

  void reset(Chip::ColorType turn);
  void update();
  bool handle_turn(const SDL_Event& event);
  void events(unsigned short stol, unsigned short strok);
  void gameOver();
  void rise_gameOver();

  short r_gameOver() const { return _gameOverCnt; }
  Chip::ColorType currentTurn() const { return _turn;}
  unsigned short currentColumn()  const { return _curCol; }
  unsigned short currentRow() const { return _curRow; }

  class GameOver {};

private:
  
  void init();  
  void setInitialChips();
  void fillBackground();
  
  void addAndTurn(int row, int col, int color);
  int turnRight(int row, int col, int color);
  int turnLeft(int row, int col, int color);
  int turnUp(int row, int col, int color);
  int turnDown(int row, int col, int color);
  int turnUpLeft(int row, int col, int color);
  int turnUpRight(int row, int col, int color);
  int turnDownRight(int row, int col, int color);
  int turnDownLeft(int row, int col, int color);
  
  bool checkTurn(Chip::ColorType hod);
  bool checkTurnUp(int row, int col, Chip::ColorType hod);
  bool checkTurnRight(int row, int col, Chip::ColorType hod);
  bool checkTurnDown(int row, int col, Chip::ColorType hod);
  bool checkTurnLeft(int row, int col, Chip::ColorType hod);
  bool checkTurnUpRight(int row, int col, Chip::ColorType hod);
  bool checkTurnDownRight(int row, int col, Chip::ColorType hod);
  bool checkTurnDownLeft(int row, int col, Chip::ColorType hod);
  bool checkTurnUpLeft(int row, int col, Chip::ColorType hod);
  
  void handle_noturn();
  
  void updateScoreField();
  
private:
  
  Chip _field[FIELD_ROWS][FIELD_COLS];
  CellState _state[FIELD_ROWS][FIELD_COLS];

  int _blackCnt = 0;
  int _whiteCnt = 0;
  Chip::ColorType _turn = Chip::kBlack;
  Chip::ColorType _initTurn = Chip::kNo;
  
  unsigned short _curCol = 0;
  unsigned short _curRow = 0;
  
  SDL_Rect _scoreField;
  SDL_Rect _turnField;
  Uint32 _background = 0;

  Text _bMove;
  Text _wMove;
  Text _score;

  short _gameOverCnt = 0;

};

#endif
