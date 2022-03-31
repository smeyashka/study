#include "field.h"
#include "log.h"

#include <iostream>
using std::cout;
using std::cerr;


#define FONT_FILE "./resources/batang.ttf"

Field::Field(Chip::ColorType turn /*= Chip::kNo*/):
  _initTurn(turn)
{ 
  init();
}

Field::~Field()
{
}

//! backhround, grid, first four chips
void Field::init()
{
  _gameOverCnt=0;
  _curCol=0;
  _curRow=0;

  fillBackground();
  setInitialChips(); 
}

void Field::fillBackground()
{
  _background = SDL_MapRGB(screen()->format, 0x30, 0x00, 0x60);//0xbb,0xf0,0xdf);
  SDL_FillRect(screen(), &screen()->clip_rect, _background);

  _scoreField.x = 0;
  //  _scoreField.y = screen()->h - 60;
  _scoreField.y = 640;
  _scoreField.w = 640;
  _scoreField.h = 30;
  SDL_FillRect(screen(), &_scoreField, 0xffffff);
  
  if (_initTurn != Chip::kNo) {
    _turnField.x = 10;
    _turnField.y = _scoreField.y + 5;
    _turnField.h = 20;
    _turnField.w = 20;
    if (_initTurn == Chip::kWhite)  {
      SDL_FillRect(screen(), &_turnField, 0xe0e0e0);
    } else {
      SDL_FillRect(screen(), &_turnField, 0x0);
    }
  }

  SImage::instance()->applySurface(0, 0, SImage::instance()->grid(), screen());
}


void Field::setInitialChips()
{
  _field[3][3].setParam(2, 2+3*FIELD_SQUARE_SIDE, 2+3*FIELD_SQUARE_SIDE, SImage::instance()->white());
  _field[3][3].show(screen()); 
  _field[3][4].setParam(1, 2+4*FIELD_SQUARE_SIDE, 2+3*FIELD_SQUARE_SIDE, SImage::instance()->black());
  _field[3][4].show(screen());
  _field[4][4].setParam(2, 2+4*FIELD_SQUARE_SIDE, 2+4*FIELD_SQUARE_SIDE, SImage::instance()->white());
  _field[4][4].show(screen());
  _field[4][3].setParam(1, 2+3*FIELD_SQUARE_SIDE, 2+4*FIELD_SQUARE_SIDE, SImage::instance()->black());
  _field[4][3].show(screen());

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      _state[i][j] = kEmptyCell;
    }
  }

  _state[3][3] = kBusyCell; 
  _state[3][4] = kBusyCell;
  _state[4][4] = kBusyCell;
  _state[4][3] = kBusyCell;
  _blackCnt = 2;
  _whiteCnt = 2;

  //!score field  initialisation
  SDL_Color col = {0x30, 0x00, 0x60};
  _wMove.setColor(col);
  _wMove.setFont(FONT_FILE, 20);
  _wMove.renderMessage("White's move");
  _bMove.setColor(col);
  _bMove.setFont(FONT_FILE, 20);
  _bMove.renderMessage("Black's move");
  _score.setColor(col);
  _score.setFont(FONT_FILE, 20);
  _score.renderMessage("Black: 2  White: 2");
}


void Field::reset(Chip::ColorType turn)
{
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      _field[i][j].setParam(0, 0, 0, 0);
    }
  }
  
  _initTurn = turn;
  _turn = Chip::kBlack;
  init();
}


void Field::updateScoreField()
{
  SDL_FillRect(screen(), &_scoreField, 0xffffff);

  if (_initTurn == Chip::kWhite)  {
    SDL_FillRect(screen(), &_turnField, 0xe0e0e0);
  } else if (_initTurn == Chip::kBlack) {
    SDL_FillRect(screen(), &_turnField, 0x0);
  }  

  if (_turn == Chip::kBlack)  {
    SImage::instance()->applySurface(50, 645, _bMove.retMessage(), screen());
  } else {
    SImage::instance()->applySurface(50, 645, _wMove.retMessage(), screen());
  }
  
  SImage::instance()->applySurface(350, 645, _score.retMessage(), screen()); 
}

// update all fields
void Field::update()
{  
  SDL_FillRect(screen(), &screen()->clip_rect, _background);
  SImage::instance()->applySurface(0, 0, SImage::instance()->grid(), screen());
  
  updateScoreField();

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (_field[i][j].color() != Chip::kNo) {
	_field[i][j].show(screen());
      }
    }
  }
}

//-----------

//! add user chip and turn possible chips in all directions
void Field::addAndTurn(int row, int col, int color)
{
  int turnCnt = 0;

  turnCnt += turnRight(row, col, color);
  turnCnt += turnLeft(row, col, color);
  turnCnt += turnUp(row, col, color);
  turnCnt += turnDown(row, col, color);
  turnCnt += turnUpLeft(row, col, color);
  turnCnt += turnUpRight(row, col, color);
  turnCnt += turnDownLeft(row, col, color);
  turnCnt += turnDownRight(row, col, color);
    
  if (color == Chip::kBlack) {
    _blackCnt += turnCnt + 1;
    _whiteCnt -= turnCnt;
  }
  else {
    _whiteCnt += turnCnt + 1;
    _blackCnt -= turnCnt;
  }
}


int Field::turnRight(int row, int col, int color)
{
  int turnCnt = 0;

  int ecol = col + 1;
  while (ecol < FIELD_COLS && _field[row][ecol].color() != color) {
    if (_field[row][ecol].color() == Chip::kNo) { ecol = FIELD_COLS; }
    ++ecol;
  }
 
  if (ecol > col + 1 && ecol < FIELD_COLS && _field[row][ecol].color() == color) {
    for (int cidx = col + 1; cidx < ecol; cidx++) {
      _field[row][cidx].turn();
      _field[row][cidx].showTurn(screen());
      ++turnCnt;
    }
  }

  return turnCnt;
}


int Field::turnLeft(int row, int col, int color)
{
  int turnCnt = 0;

  int scol = col - 1;
  while (scol >= 0 && (_field[row][scol].color() != color)) {
    if (_field[row][scol].color() == Chip::kNo) { scol = -1; }
    --scol;
  }
  
  if (scol < col-1 && scol >= 0 && _field[row][scol].color() == color) {
    for (int cidx = col - 1; cidx > scol; cidx--) {
      _field[row][cidx].turn();
      _field[row][cidx].showTurn(screen());
      ++turnCnt;
    }
  }

  return turnCnt;
}


int Field::turnUp(int row, int col, int color)
{
  int turnCnt = 0;

  int srow = row - 1;
  while (srow >= 0 && _field[srow][col].color() != color) {
    if (_field[srow][col].color() == Chip::kNo) { srow = -1; }
    --srow;
  }
  if (srow < row - 1 && srow >= 0 && _field[srow][col].color() == color) { 
    for (int ridx = row - 1; ridx > srow; ridx--) {
      _field[ridx][col].turn();
      _field[ridx][col].showTurn(screen());
      ++turnCnt;
    }
  }

  return turnCnt;
}


int Field::turnDown(int row, int col, int color)
{
  int turnCnt = 0;

  int erow = row + 1;
  while (erow < FIELD_ROWS && _field[erow][col].color() != color) {
    if (_field[erow][col].color() == Chip::kNo) { erow = FIELD_ROWS; }
    ++erow;
  }
  
  if (erow > row + 1 && erow < FIELD_ROWS && _field[erow][col].color() == color) {
    for (int ridx = row + 1; ridx < erow; ridx++) {
      _field[ridx][col].turn();
      _field[ridx][col].showTurn(screen());
      ++turnCnt;
    }
  }
  
  return turnCnt;
}


int Field::turnUpLeft(int row, int col, int color)
{
  int turnCnt = 0;

  int srow = row - 1;
  int scol = col - 1;
  while (srow >= 0 && scol >= 0 && _field[srow][scol].color() != color) {
    if (_field[srow][scol].color() == Chip::kNo) { srow = -1; }
    --srow;
    --scol;
  }
  
  if (srow < row - 1 && srow >= 0 &&
      scol < col - 1 && scol >= 0 &&
      _field[srow][scol].color() == color) {
    for (int ridx = row - 1, cidx = col - 1; ridx > srow && cidx > scol; ridx--, cidx--) {
      _field[ridx][cidx].turn();
      _field[ridx][cidx].showTurn(screen());
      ++turnCnt;
    }
  }
  
  return turnCnt;
}


int Field::turnUpRight(int row, int col, int color)
{
  int turnCnt = 0;

  int srow = row - 1;
  int ecol = col + 1;
  while (ecol < FIELD_ROWS && srow > 0 && _field[srow][ecol].color() != color) {
    if (_field[srow][ecol].color() == Chip::kNo) { srow = -1; }
    --srow;
    ++ecol;
  }
  
  if (srow < row - 1 && srow >= 0 &&
      ecol > col + 1 && ecol < FIELD_ROWS &&
      _field[srow][ecol].color() == color) {
    for (int ridx = row - 1, cidx = col + 1; ridx > srow && cidx < ecol; ridx--, cidx++) {
      _field[ridx][cidx].turn();
      _field[ridx][cidx].showTurn(screen());
      ++turnCnt;
    }
  }

  return turnCnt;
}


int Field::turnDownRight(int row, int col, int color)
{
  int turnCnt = 0;

  int erow = row + 1;
  int ecol = col + 1;
  while ((_field[erow][ecol].color() != color) && erow < FIELD_ROWS && ecol < FIELD_ROWS) {
    if (_field[erow][ecol].color() == Chip::kNo) { erow = FIELD_ROWS; }
    ++erow;
    ++ecol;
  }
  if (erow > row + 1 && erow < FIELD_ROWS &&
      ecol > col + 1 && ecol < FIELD_ROWS &&
      _field[erow][ecol].color() == color) {
    for (int ridx = row + 1, cidx = col + 1; ridx < erow && cidx < ecol; ridx++, cidx++) {
      _field[ridx][cidx].turn();
      _field[ridx][cidx].showTurn(screen());
      ++turnCnt;
    }
  }

  return turnCnt;
}


int Field::turnDownLeft(int row, int col, int color)
{
  int turnCnt = 0;

  int erow = row + 1;
  int scol = col - 1;
  while ((_field[erow][scol].color() != color) && erow < FIELD_ROWS && scol > 0) {
    if (_field[erow][scol].color() == Chip::kNo) { erow = FIELD_ROWS; }
    ++erow;
    --scol;
  }
  
  if (erow > row + 1 && erow < FIELD_ROWS &&
      scol < col - 1 && scol >= 0 &&
      _field[erow][scol].color() == color) {
    for (int ridx = row + 1, cidx = col - 1; ridx < erow && cidx > scol; ridx++, cidx--) {
      _field[ridx][cidx].turn();
      _field[ridx][cidx].showTurn(screen());
      ++turnCnt;
    }
  }
  
  return turnCnt;
}


//-----------

bool Field::checkTurn(Chip::ColorType hod)
{
  bool flag = false; //flag of at least one available position

  for (int row = 0; row < FIELD_ROWS; row++) {
    for (int col = 0; col < FIELD_ROWS; col++) {
      if (_state[row][col] != kBusyCell) {
	
	//check the possibility of flipping chips in one of the directions
	//if yes => fl=true, set allowed sign to cell; checking the next direction 
	
	if (checkTurnUp(row, col, hod) ||
	    checkTurnRight(row, col, hod) ||
	    checkTurnDown(row, col, hod)  ||
	    checkTurnLeft(row, col, hod)) {
	  flag =  true;
	  continue;
	}

	if (checkTurnUpRight(row, col, hod) ||
	    checkTurnDownRight(row, col, hod) ||
	    checkTurnDownLeft(row, col, hod) ||
	    checkTurnUpLeft(row, col, hod)) {
	  flag =  true;
	  continue;
	}

	//if no possibility directions
	_state[row][col] = kEmptyCell;
      }
    }
  }

  return flag;
}


bool Field::checkTurnUp(int row, int col, Chip::ColorType hod)
{
  int srow = row - 1;
  while (srow > 0 && _field[srow][col].color() != hod && _field[srow][col].color() != Chip::kNo) {
    --srow;
  }
  
  if (srow < (row - 1) && srow >= 0 && _field[srow][col].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }

  return false;
}

bool Field::checkTurnRight(int row, int col, Chip::ColorType hod)
{
  int ecol = col + 1;
  while (ecol < FIELD_ROWS && _field[row][ecol].color() != hod && _field[row][ecol].color() != Chip::kNo) {
    ++ecol;
  }
  
  if (ecol > (col + 1) && ecol < FIELD_ROWS && _field[row][ecol].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }
  
  return false;
}


bool Field::checkTurnDown(int row, int col, Chip::ColorType hod)
{
  int erow = row + 1;
  while (erow < FIELD_ROWS && _field[erow][col].color() != hod && _field[erow][col].color() != Chip::kNo) {
    ++erow;
  }
  
  if (erow > (row + 1) && erow < FIELD_ROWS && _field[erow][col].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }

  return false;
}

bool Field::checkTurnLeft(int row, int col, Chip::ColorType hod)
{
  int scol = col - 1;
  while (scol > 0 && _field[row][scol].color() != hod && _field[row][scol].color() != Chip::kNo) {
    --scol;
  }
  
  if (scol < (col - 1) && scol >= 0 && _field[row][scol].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }
  
  return false;
}

bool Field::checkTurnUpRight(int row, int col, Chip::ColorType hod)
{
  int srow = row - 1;
  int ecol = col + 1;
  while (srow > 0 && ecol < FIELD_ROWS && _field[srow][ecol].color() != hod && _field[srow][ecol].color() != Chip::kNo) {
    --srow;
    ++ecol;
  }
  
  if (srow < row - 1 && srow >= 0 && ecol < FIELD_ROWS && _field[srow][ecol].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }
  
  return false;
}

bool Field::checkTurnDownRight(int row, int col, Chip::ColorType hod)
{
  int erow = row + 1;
  int ecol = col + 1;
  while (erow < FIELD_ROWS && ecol < FIELD_ROWS && _field[erow][ecol].color() != hod && _field[erow][ecol].color() != Chip::kNo) {
    ++erow;
    ++ecol;
  }
  
  if (erow > row + 1 && erow < FIELD_ROWS && ecol < FIELD_ROWS && _field[erow][ecol].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }
  
  return false;
}

bool Field::checkTurnDownLeft(int row, int col, Chip::ColorType hod)
{
  int erow = row + 1;
  int scol = col - 1;
  while (erow < FIELD_ROWS && scol > 0 && _field[erow][scol].color() != hod && _field[erow][scol].color() != Chip::kNo) {
    ++erow;
    --scol;
  }
  
  if (erow > row + 1 && erow < FIELD_ROWS && scol >= 0 && _field[erow][scol].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }
  
  return false;
}

bool Field::checkTurnUpLeft(int row, int col, Chip::ColorType hod)
{
  int srow = row - 1;
  int scol = col - 1;
  while (srow >= 0 && scol >= 0 && _field[srow][scol].color() != hod && _field[srow][scol].color() != Chip::kNo) {
    --srow;
    --scol;
  }

  if (srow < row - 1 && srow >= 0 && scol >= 0 && _field[srow][scol].color() == hod) {
    _state[row][col] = kAllowedCell;
    return true;
  }
  
  return false;
}

//-----------

void Field::rise_gameOver() 
{ 
  //  cout<<"смена хода\n";
  _gameOverCnt++; 
  errlog << "gameOver=" << _gameOverCnt << " turn = " << _turn << std::endl;

  _turn = (_turn == Chip::kBlack) ? Chip::kWhite : Chip::kBlack;

  updateScoreField();

  if (SDL_Flip(screen()) == -1) {
    throw Err_Init();
  }
}

void Field::handle_noturn()
{
  cout << "No possible positions turn = " << _turn << std::endl;
  
  _gameOverCnt++;
  if (_gameOverCnt >= 2) {
    _gameOverCnt = 2;
    throw GameOver();
  }
  
  _turn = (_turn == Chip::kBlack) ? Chip::kWhite : Chip::kBlack;
  
  updateScoreField();
  
  if (SDL_Flip(screen()) == -1) {
    throw Err_Init();
  }
}

bool Field::handle_turn(const SDL_Event& event)
{
  // no possible positions => turn = other_color;
  
  if (!checkTurn(_turn)) {
    handle_noturn();    
    return false;
  }

  _gameOverCnt = 0;

  if (event.type != SDL_MOUSEBUTTONDOWN || event.button.button != SDL_BUTTON_LEFT) {
    return false;
  }

  int col = event.button.x / FIELD_SQUARE_SIDE;
  int row = event.button.y / FIELD_SQUARE_SIDE;
  errlog << "col=" << col << " row=" << row << std::endl;

  if (col < 0 || col > FIELD_ROWS || row < 0 || row > FIELD_ROWS) {
    return false;
  }

  errlog << "sate=" << _state[row][col] << " "  << kAllowedCell << std::endl;
  
  if (_state[row][col] != kAllowedCell) {
    return false;
  }

  _curCol = col;
  _curRow = row;

  return true;
}

void Field::events(unsigned short col, unsigned short row)
{
  _gameOverCnt = 0;

  _curCol = col;
  _curRow = row;

  if (_turn == Chip::kBlack) { 
    _field[_curRow][_curCol].setParam(1, _curCol*FIELD_SQUARE_SIDE+2, _curRow*FIELD_SQUARE_SIDE+2, SImage::instance()->black());
    _turn = Chip::kWhite;
  } else {
    _field[_curRow][_curCol].setParam(2, _curCol*FIELD_SQUARE_SIDE+2, _curRow*FIELD_SQUARE_SIDE+2, SImage::instance()->white());
    _turn = Chip::kBlack;
  }

  _field[_curRow][_curCol].show(screen());
  _state[_curRow][_curCol] = kBusyCell;

  errlog << "cur " << _curRow << " "  << _curCol << "\n";

  addAndTurn(_curRow, _curCol, _field[_curRow][_curCol].color());

  std::string scoreLine =
    "Black: " + std::to_string(_blackCnt) +
    "  White: " + std::to_string(_whiteCnt);
  
  _score.renderMessage(scoreLine.c_str());

  updateScoreField();

  if (SDL_Flip(screen()) == -1) {
    throw Err_Init();
  }
}

void Field::gameOver()
{
  std::string scoreLine =
    "Black: " + std::to_string(_blackCnt) +
    "  White: " + std::to_string(_whiteCnt);

  SDL_Color col = {0x00, 0xff, 0xff};
  _score.setColor(col);
  _score.renderMessage(scoreLine.c_str());
  
  SDL_Rect sch;
  sch.x = (screen()->w - _score.w()) / 2 - 20;
  sch.y = 150;
  sch.w = _score.w() + 40;
  sch.h = 100;
  SDL_FillRect(screen(), &sch, 0x000000);
  //SDL_FillRect(screen(), &sch, 0xffffff);

  SImage::instance()->applySurface((screen()->w - _score.w()) / 2, 190, _score.retMessage(), screen());

  col = {0x30, 0x00, 0x60};
  _score.setColor(col);
  _score.renderMessage(scoreLine.c_str());
  
  if (SDL_Flip(screen()) == -1) {
    throw Err_Init();
  }
}
