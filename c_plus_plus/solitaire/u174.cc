#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <deque>
#include <list>


#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

//NOTE It would be nice to add:
// - a double click
// - automatic layout when all the cards are open
// - animation when one win
// - true random


#define dbglog   std::cerr << __FILE__ << ':' << __LINE__ << ": "
#define errlog   std::cerr << __FILE__ << ':' << __LINE__ << ": "


using namespace std;

union Color_t {
  struct cc {
    unsigned int a:8;
    unsigned int b:8;
    unsigned int g:8;
    unsigned int r:8;
  } rgba;
  unsigned int col;
};



enum Cards { kFace_C6 = 0,  Face_C7 = 1,  Face_C8 = 2,  Face_C9 = 3,  Face_C10 = 4,  Face_CJ = 5,  Face_CQ = 6,  Face_CK = 7,  Face_CA = 8,  //треф
	     kFace_D6 = 9,  Face_D7 = 10, Face_D8 = 11, Face_D9 = 12, Face_D10 = 13, Face_DJ = 14, Face_DQ = 15, Face_DK = 16, Face_DA = 17, //бубен
	     kFace_P6 = 18, Face_P7 = 19, Face_P8 = 20, Face_P9 = 21, Face_P10 = 22, Face_PJ = 23, Face_PQ = 24, Face_PK = 25, Face_PA = 26, //вини 
	     kFace_H6 = 27, Face_H7 = 28, Face_H8 = 29, Face_H9 = 30, Face_H10 = 31, Face_HV = 32, Face_HD = 33, Face_HK = 34, Face_HT = 35, //черви
	     kSuitCard = 36, kCardsCount = 36 };

enum Suit {
  CLUBS  = 0,
  DIAMON = 1,
  PIKES  = 2 ,
  HEARTS = 3,
  EMPTY  = 4
};


/*****/


class Window {

 public:
  class Err_Init {};

  Window(int ww = 710, int hh = 580, unsigned int bc = 0x14446f);
  ~Window() {}
  
  void update() { if (SDL_Flip(_screen) == -1) throw Err_Init(); }
  int widthScreen()  { return _width; }
  int heightScreen() { return _height; }
 
  unsigned int color_ui() { return _background.col; }

protected:
  SDL_Surface* _screen;

private:
  int _width;
  int _height;
  int _bpp;

  int _old_width;
  int _old_height;

  Color_t _background;
};

Window::Window(int ww, int hh, unsigned int bc) :
  _width(ww),
  _height(hh),
  _bpp(32)
{
  _background.col = bc;

  _old_width = _width;
  _old_height = _height;
  
  _screen = SDL_SetVideoMode(_width, _height, _bpp, SDL_SWSURFACE /*| SDL_OPENGL *//*| SDL_RESIZABLE*/);
  
  if (_screen == nullptr) {
    throw Err_Init();
  }

  SDL_WM_SetCaption("Solitaire", NULL);

}

// void Window::resize(int ww,int hh)
// {
//   width_screen=ww;
//   heigt_screen=hh;

//   screen=SDL_SetVideoMode(width_screen,height_screen,bpp_screen, SDL_SWSURFACE/* | SDL_OPENGL*/ | SDL_RESIZABLE);
//   if (screen==0) throw Err_Init();
// }

/***/

//load card's image
class Image {
public:
  Image();
  ~Image() { if (_all_cards != nullptr) SDL_FreeSurface(_all_cards); }

  static SDL_Surface* loadImage(std::string filename);
  static void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = nullptr);
  
  SDL_Rect clips(Cards num);
  SDL_Surface* allCards() { return _all_cards; }
  int width()  { return _clip.w; }
  int height() { return _clip.h; }

private:
  bool loadFiles();
  void setClip();
  
private:
  SDL_Rect _clip;
  SDL_Surface* _all_cards;

};

Image::Image()
{
  loadFiles();
  setClip();
}

void Image::setClip()
{
  _clip.x = 0;
  _clip.y = 0;
  _clip.w = 79;
  _clip.h = 123;
}

bool Image::loadFiles()
{
  _all_cards = loadImage("./paris_ink.png");
  
  if (_all_cards == nullptr) {
    return false;
  }
  
  return true;
}

SDL_Surface* Image::loadImage(std::string filename)
{
  SDL_Surface* optimizedImage = nullptr;
  SDL_Surface* loadedImage = IMG_Load(filename.c_str());
  
  if (loadedImage != nullptr) {
    optimizedImage = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
    
    if (optimizedImage != nullptr) {
      Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0x14, 0x44, 0x6f);  
      SDL_SetColorKey(optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
    }
  }
  
  return optimizedImage;
}

void Image::applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
  SDL_Rect offset;  
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface(source, clip, destination, &offset);
}


SDL_Rect Image::clips(Cards num)
{ 
  SDL_Rect kk; 
  kk.w = _clip.w; 
  kk.h = _clip.h;
  
  if (num == 36)    { kk.x = 0; }
  else if (num < 9) { kk.x = num * kk.w; }
  else if (num < 18) { kk.x = (num -  9) * kk.w; }
  else if (num < 27) { kk.x = (num - 18) * kk.w; }
  else                     { kk.x = (num - 27) * kk.w; }
  
  kk.y = int(num) / 9 * kk.h;
  
  return kk; 
}

/***/

struct CardWithProp {
  Cards card;
  bool open;
  CardWithProp(Cards c, bool pr): card(c), open(pr) {}  
};

enum CardColor {
  kBlack = 0,
  kRed = 1
};

class Solitaire : private Image, public Window {
public:

  enum cursor_t {
    kEmpty  = -2,
    kPack   = -1,
    kColumn1 = 0,
    kColumn2 = 1,
    kColumn3 = 2,
    kColumn4 = 3,
    kColumn5 = 4
  };
  
  Solitaire();
  ~Solitaire();

  void newLayout();
  void update();

  void events(SDL_Event& event);
  

  inline Suit suit(Cards a) const;
  CardColor color(Cards a) const;

private:

  bool neib_and_less(Cards a, Cards b) const; //b меньше a на 1
  bool neib_and_more(Cards a, Cards b) const; //b больше a на 1

  bool push_from_deck_to_cursor();
  bool push_from_stack_to_cursor(int from, int kol);
  bool push_to_base_from_cursor(int to);
  bool push_to_stack_from_cursor(int to);
  bool push_to_emptystack_from_cursor(int to);
  bool push_to_fillstack_from_cursor(int to);
  bool push_back_from_cursor();

  void next_card();

  

  bool mousePressed(int x_curs, int y_curs, short num_r);
  void mouseReleased(int x_curs, int y_curs, short num_r);
  
  void updateClose();
  void updateOpen();
  void updateBase();
  void updateLayout();
  void fillAbsentCard(int x, int y);
  
  short stackNumber(int x_curs);
  
private:
  deque<Cards> _deckClose;
  deque<Cards> _deckOpen; //!!! max_kol = 36 - 15
  short _openCnt = 0;

  stack<Cards> _base[4];

  vector<CardWithProp> _cardStacks[5]; //!!! max_kol = 9 + 5

  int _x_close;
  int _y_close;
  int _d_close_open;
  int _x_base;
  int _d_base;
  int _x_stack;
  int _y_stack;
  float _koef_h;

  cursor_t _type_cursor;
  list<Cards> _cursor; // max = 9  
};


Solitaire::Solitaire()
{
  for (int i = 0; i < kCardsCount; i++) {
    _deckClose.push_back(Cards(i));
  }
  random_shuffle(_deckClose.begin(), _deckClose.end());

  _openCnt = 0;
    

  for (int j = 0; j < 5; j++) {
    _cardStacks[j].reserve(15);
    for (int i = 0; i <= 4 - j; i++) { 
      if (i != 4 - j) {
	_cardStacks[j].push_back(CardWithProp(_deckClose.back(), 0));
      } else {
	_cardStacks[j].push_back(CardWithProp(_deckClose.back(), 1));
      }
      _deckClose.pop_back();
    }
  }

  _x_close = 10;
  _y_close = 20;
  _d_close_open = 10;
  _x_base  = _x_close + 2 * width() + _d_close_open + 60;
  _d_base  = 30;
  _x_stack = 10 + width() + 90;
  _y_stack = 70 + height();
  _koef_h  = 0.2;

  _type_cursor = kEmpty;
}

Solitaire::~Solitaire()
{
}


void Solitaire::newLayout()
{
  _deckClose.clear();
  while (!_deckOpen.empty()) {
    _deckOpen.clear();
  }
  
 _openCnt = 0;

  for (int i = 0; i < 4; i++) {
    while (!_base[i].empty()) {
      _base[i].pop();
    }
  }
  for (int i = 0; i < 5; i++) {
    _cardStacks[i].clear();
  }

  for (int i = 0; i < kCardsCount; i++) {
    _deckClose.push_back(Cards(i));
  }
  random_shuffle(_deckClose.begin(), _deckClose.end());

  for (int num = 0; num < 5; num++) {
    for (int idx = 0; idx <= 4 - num; idx++) { 
      if (idx != 4 - num) {
	_cardStacks[num].push_back(CardWithProp(_deckClose.back(), false));
      } else {
	_cardStacks[num].push_back(CardWithProp(_deckClose.back(), true));
      }
      _deckClose.pop_back();
    }
  }
}

// fill with color the field for card
void Solitaire::fillAbsentCard(int x, int y)
{
  SDL_Rect kk; 
  kk.x = x;
  kk.y = y;
  kk.w = Image::width(); 
  kk.h = Image::height();
  SDL_FillRect(_screen, &kk, 0x154977);
}
			 
// upadate all layouts with new state
void Solitaire::update()
{
  dbglog << "update\n";
  SDL_FillRect(_screen, &_screen->clip_rect, Window::color_ui());
  updateClose();
  updateOpen();
  updateBase();
  updateLayout();
}

// update the closed part of the deck 
void Solitaire::updateClose()
{
  SDL_Rect rec;

  if (_deckClose.empty()) {
    fillAbsentCard(_x_close, _y_close);
  } else {
    rec = clips(kSuitCard);
    applySurface(_x_close, _y_close, allCards(), _screen, &rec);
  }
}

// update the opened part of the deck 
void Solitaire::updateOpen()
{
  SDL_Rect rec;
  if (_deckOpen.empty()) {
    fillAbsentCard(_x_close + width() + _d_close_open, _y_close);
  } else {
    dbglog << "open_size = " << _deckOpen.size() << " cnt = " << _openCnt << endl;
    if ((uint)_openCnt > _deckOpen.size()) {
      errlog << "Open cards count > cards in deck\n";
      _openCnt = _deckOpen.size();
    }
    for (int idx = 0; idx < _openCnt; ++idx) {
      // dbglog << "index  " << _deckOpen.size() -_openCnt + idx << endl;
      rec = clips(Cards(_deckOpen[_deckOpen.size() -_openCnt + idx]));
      applySurface(_x_close + width() + _d_close_open + int(idx * 0.2 * width()), _y_close, allCards(), _screen, &rec);
    }
  }
}


// update the closed part of the deck 
void Solitaire::updateBase()
{
  SDL_Rect rec;
  for (int idx = 0; idx < 4; idx++) {
    if (_base[idx].empty()) {
      fillAbsentCard(_x_base + idx * (width() + _d_base), _y_close);
    } else {
      rec = clips(_base[idx].top());
      applySurface(_x_base + idx*(width() + _d_base), _y_close, allCards(), _screen, &rec);
    }
  }
}

// update stacks
void Solitaire::updateLayout()
{
  SDL_Rect rec;
  for (int idx = 0; idx < 5; idx++) {
    if (_cardStacks[idx].empty()) {
      fillAbsentCard(_x_stack + idx*(width() + _d_base), _y_stack);
     
    } else {
      
      for (size_t cnt = 0; cnt < _cardStacks[idx].size(); cnt++) {
	if (_cardStacks[idx][cnt].open) {
	  rec = clips(_cardStacks[idx][cnt].card);
	} else {
	  rec = clips(Cards(kSuitCard));
	}

	applySurface(_x_stack + idx*(width() + _d_base),
		     _y_stack + cnt*(int)(_koef_h*height()),
		     allCards(), _screen, &rec);
      }
    }
  }
}


short Solitaire::stackNumber(int x_curs)
{
  if (x_curs >= _x_stack && x_curs < _x_stack + width()) return 0;
  if (x_curs >= _x_stack +   width() +   _d_base && x_curs < _x_stack + 2*width() +   _d_base) return 1;
  if (x_curs >= _x_stack + 2*width() + 2*_d_base && x_curs < _x_stack + 3*width() + 2*_d_base) return 2;
  if (x_curs >= _x_stack + 3*width() + 3*_d_base && x_curs < _x_stack + 4*width() + 3*_d_base) return 3;
  if (x_curs >= _x_stack + 4*width() + 4*_d_base && x_curs < _x_stack + 5*width() + 4*_d_base) return 4;

  return -1;
}

Suit Solitaire::suit(Cards a) const
{
  return Suit(int(a) / 9);
}

CardColor Solitaire::color(Cards a) const //0-black, 1-red
{
  int num = int(a) / 9;
  switch (num) {
  case 1: case 3:
    return kRed;
  case 0: case 2:
    return kBlack;
  default: {}
  }
  return kRed;
}

bool Solitaire::neib_and_less(Cards a, Cards b) const
{
  // dbglog << "a=" << a << " b = " << b << endl;

  int c = int(a % 9);
  int d = int(b % 9);

  if (c != 0) {
    if ((c - 1) == d) return true;
  } else {
    if ((c + 8) == d) return true;
  }
  
  return false;
}

bool Solitaire::neib_and_more(Cards a, Cards b)const
{
  int c = int(a);
  if (c != Face_CA && c != Face_DA && c != Face_PA && c != Face_HT ) {
    if ((c + 1) == int(b) ) return true;
  } else {
    if ((c - 8) == int(b)) return true;
  }
  
  return false;
}


bool Solitaire::push_from_deck_to_cursor()
{
  if (_deckOpen.empty()) return false;

  Cards card = _deckOpen.back();
  dbglog << "card = " << card << endl;
  
  _cursor.push_back(card);
  _deckOpen.pop_back();
  
  dbglog << "cursor.zise = " << _cursor.size() << endl;

  
  --_openCnt;
  if (_openCnt == 0 && !_deckOpen.empty()) {
    _openCnt = _deckOpen.size() >= 3 ? 3 : _deckOpen.size();
  }
  dbglog << "opencnt "  << _openCnt << " " << _deckOpen.size() << endl;
  
  _type_cursor = kPack;
  
 return true;
}

bool Solitaire::push_from_stack_to_cursor(int from, int kol)
{
  if (_cardStacks[from].empty()) return false;

  int kol_p = 0;

  while (!_cardStacks[from].empty() && kol_p < kol && _cardStacks[from].back().open) {
    dbglog << "card = " << _cardStacks[from].back().card << endl;
    _cursor.push_front(_cardStacks[from].back().card);
    _cardStacks[from].pop_back();
    kol_p++;
  }
  
  _type_cursor = cursor_t(from);

  dbglog << "type_k=" << _type_cursor << endl;

  return true;
}


bool Solitaire::push_to_base_from_cursor(int to)
{
  if (_cursor.empty()) return false;
  if (_cursor.size() > 1) return false;

  bool ok = false;

  Cards card = _cursor.back();
  
  if (_base[to].empty()) {

    if (card != Face_CA && card != Face_DA && card != Face_PA && card != Face_HT) {
      return false;
    }

    _base[to].push(card);
    _cursor.pop_back();

    dbglog << "type "  << _type_cursor << endl;
    if (_type_cursor >= kColumn1 && !_cardStacks[_type_cursor].empty()) {
      _cardStacks[_type_cursor].back().open = true;
    }

    dbglog << "size_k = " << _cursor.size() << endl;
    _type_cursor = kEmpty;
    ok = true;
    
  } else {
    
    Cards card_base = _base[to].top();
    
    if (suit(card) == suit(card_base) && neib_and_more(card_base, card)) {
      dbglog << "ok to base " << _base[0].size() << " " << _base[1].size() << " "
	     << _base[2].size() << " " << _base[3].size() << endl;
      _base[to].push(card);
      _cursor.pop_back();

      dbglog << "type cursor " << _type_cursor << endl;
      
      if (_type_cursor >= kColumn1 && !_cardStacks[_type_cursor].empty()) {
	_cardStacks[_type_cursor].back().open = true;
      }
      
      _type_cursor = kEmpty;
      ok = true;
    }
  }
  
  return ok;
}


bool Solitaire::push_to_stack_from_cursor(int to)
{
  dbglog << "push_to_stack_from_cursor\n";
  if (_cursor.empty()) return false;

  bool ok = false;
  
  dbglog << "to = " << to << " size = " << _cardStacks[to].size() << " cursor size " << _cursor.size()  << endl;
  
  if ( _cardStacks[to].empty()) {
    ok = push_to_emptystack_from_cursor(to);   
  } else {
    ok = push_to_fillstack_from_cursor(to);
  }

  return ok;
}

bool Solitaire::push_to_emptystack_from_cursor(int to)
{
  Cards card = _cursor.front();
  dbglog << "stack = empty, card = " << card << endl;
  if (card != Face_CK && card != Face_DK && card != Face_PK && card != Face_HK) {
    return false;
  }
  
  while (!_cursor.empty()) {
    //check suit
    dbglog << "push to stack" << to << " card = " << _cursor.front() << endl;
    _cardStacks[to].push_back(CardWithProp(_cursor.front(), 1));
    _cursor.pop_front();      
  }
  
  if (_type_cursor >= kColumn1 && !_cardStacks[_type_cursor].empty()) {
    _cardStacks[_type_cursor].back().open = true;
  }
  
  _type_cursor = kEmpty;
  
  return true;
}


bool Solitaire::push_to_fillstack_from_cursor(int to)
{
  bool ok = false;
  
  dbglog << "stack != empty\n";
  
  Cards card = _cursor.front();
  Cards card_base = _cardStacks[to].back().card;
  
  dbglog << "suit_base = " << suit(card_base) << " suit_card=" << suit(card) << endl;
  dbglog << "neib = " << neib_and_less(card_base, card) << endl;
  
  CardColor color_curs = color(card);
  CardColor color_base = color(card_base);
  
  if (color_curs != color_base && neib_and_less(card_base, card)) {
    
    while (!_cursor.empty()) {
      dbglog << "push  to stack " << to << " card = " << _cursor.front() << " " << _cursor.size()  << endl;
      dbglog << "s = " << _cardStacks[to].size() << " " << _cardStacks[to].at(_cardStacks[to].size() - 1).card << " " << endl;
      _cardStacks[to].push_back(CardWithProp(_cursor.front(), 1));
      dbglog << "s = " << _cardStacks[to].size() << endl;
      _cursor.pop_front();
    }
    
    if (_type_cursor >= kColumn1 && !_cardStacks[_type_cursor].empty()) {
      _cardStacks[_type_cursor].back().open = true;
    }
    
    _type_cursor = kEmpty;
    ok = true;
  }
  
  return ok;
}


bool Solitaire::push_back_from_cursor()
{
  if (_cursor.empty()) return false;

  switch(_type_cursor) {
  case kPack:
    _deckOpen.push_back(_cursor.back());
    _cursor.pop_back();
    ++_openCnt;
    dbglog << "openCnt " << _openCnt << endl;
    break;
  case kColumn1:  case kColumn2: case kColumn3: case kColumn4: case kColumn5:
    while (!_cursor.empty()) {
      _cardStacks[_type_cursor].push_back(CardWithProp(_cursor.front(), 1));
      _cursor.pop_front();
    }
    break;
  default: return false;
  }
 
  _type_cursor = kEmpty;
  return true;
}


void Solitaire::next_card()
{
  if (_deckClose.empty()) {
    while (!_deckOpen.empty()) {
      _deckClose.push_back(_deckOpen.back());
      _deckOpen.pop_back();
    }
  } else {
    for (int i = 0; i < 3; i++) {
      if (!_deckClose.empty()) {
	_deckOpen.push_back(_deckClose.back());
	_deckClose.pop_back();
      }
    }
  }

  _openCnt = _deckOpen.size() >= 3 ? 3 : _deckOpen.size();
  dbglog << "openCnt " << _openCnt << endl;  
}


bool Solitaire::mousePressed(int x_curs, int y_curs, short num_r)
{
  //select closed deck
  if (x_curs >= (_x_close) &&
      x_curs < (_x_close + width() + _d_close_open) &&
      y_curs >= _y_close && y_curs < _y_close + height()) {
    dbglog << "deck_close\n";
    next_card();
    return true;
  }
  
  //select opened deck
  if (x_curs >= (_x_close + width() + _d_close_open) &&
      x_curs < (_x_close + 2*width() + _d_close_open + int(2*0.2*width())) &&
      y_curs >= _y_close && y_curs < _y_close + height()) {
    dbglog << "deck_open\n";
    push_from_deck_to_cursor();
    return true;
  }
  
  //select stacks
  if (y_curs >= _y_stack) {
    unsigned short num_k = (int)((y_curs - _y_stack) / (_koef_h * height()));
    
    num_r = stackNumber(x_curs);
    if (num_r != -1) {
      dbglog << "stack " << num_r << "\n";
      dbglog << "num_k = " << num_k << " size = " << _cardStacks[num_r].size() << " k = " << (1/_koef_h-1)
	     << " res = " << (_cardStacks[num_r].size() + 1/_koef_h - 1) << endl;
      if (_cardStacks[num_r].size() == 0) {
      } else if (_cardStacks[num_r].size() >= unsigned(num_k + 1)) {
	dbglog << "card " << num_k << endl;
	if (_cardStacks[num_r][num_k].open) {
	  push_from_stack_to_cursor(num_r, _cardStacks[num_r].size() - num_k);
	}
      } else if ((_cardStacks[num_r].size() + 1/_koef_h - 1) > num_k) {
	dbglog << "card last " << (num_k + 1 - 1/_koef_h) << endl;
	push_from_stack_to_cursor(num_r, 1);
      }
    }
    dbglog << "cursor = " << _cursor.size() << endl;
    return true;
  }    

  return false;
}

void Solitaire::mouseReleased(int x_curs, int y_curs, short num_r)
{
  //base
  if (y_curs >= _y_close && y_curs < (_y_close + height())) {
    bool fl = false;
    if (x_curs >= _x_base && x_curs < (_x_base + width())) { //1
      dbglog << "base 1\n";
      fl = push_to_base_from_cursor(0);
    } else if (x_curs >= (_x_base + width() + _d_base) && x_curs < (_x_base + 2*width() + _d_base)) { //2
      dbglog << "base 2\n";
      fl = push_to_base_from_cursor(1);
    } else if (x_curs >= (_x_base + 2*width() + 2*_d_base) && x_curs < (_x_base + 3*width() + 2*_d_base)) {//3
      dbglog << "base 3\n";
      fl = push_to_base_from_cursor(2);
    } else if (x_curs >= (_x_base + 3*width() + 3*_d_base) && x_curs < (_x_base + 4*width() + 3*_d_base)) {//4
      dbglog << "base 4\n";
      fl = push_to_base_from_cursor(3);
    }
    if (!fl) {
      push_back_from_cursor();
    }
  } else if ((num_r = stackNumber(x_curs)) != -1) {
    dbglog << "to " << num_r << endl;
    if (!push_to_stack_from_cursor(num_r)) {
      push_back_from_cursor();
    }
  } else {
    dbglog << "push_back\n";
    push_back_from_cursor();
  }

}


void Solitaire::events(SDL_Event& event)
{
  int x_curs = event.button.x;
  int y_curs = event.button.y;
  short num_r = -1;
  
  if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
    bool ok = mousePressed(x_curs, y_curs, num_r);
    if (ok) {
      update();
      Window::update();
    }
  }

  if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
    mouseReleased(x_curs, y_curs, num_r);
    update();
    Window::update();
  }
  

}

/****/
void display(Window& w, Solitaire& solitaire)
{
  solitaire.update();
  w.update();
}


int main()
{
  SDL_Event event;
  if (SDL_Init(SDL_INIT_VIDEO) == -1) exit(-1);

  Window w;
  Solitaire solitaire;

  display(w, solitaire);

  while (SDL_WaitEvent(&event) >= 0) {
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) break;
    //   if (event.type == SDL_VIDEORESIZE) {
    //       w.resize(event.resize.w, event.resize.h);
    //       display(w, solitaire);
    //     }
    if (event.type == SDL_VIDEOEXPOSE) {
      display(w, solitaire);
    }
  
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n && event.key.keysym.mod == (KMOD_LCTRL |4096)) {
      solitaire.newLayout();
      display(w, solitaire);
    }
    
    solitaire.events(event);    
  }
  
  SDL_Quit();

  return 0;
}
