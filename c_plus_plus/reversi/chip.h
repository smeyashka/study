#ifndef _CHIP_HH
#define _CHIP_HH

#include <SDL/SDL.h>
#include "image.h"


class Chip {
public:
  
  enum ColorType { kNo = 0, kBlack = 1, kWhite = 2 };

  Chip(int cc, int xx, int yy);
  Chip();
  ~Chip();

  void setParam(int cc, int xx, int yy, SDL_Surface* image) { _color = ColorType(cc); _x = xx; _y = yy; _img = image; }

  void show(SDL_Surface* screen) const { SImage::instance()->applySurface(_x, _y, _img, screen); }
  void showTurn(SDL_Surface* screen);
  
  void turn();
  
  ColorType color() { return _color; }

private:
  Chip(const Chip&);

private:  
  ColorType _color = kNo;
  int _x = 0;
  int _y = 0;
  SDL_Surface* _img = nullptr;  
  
};

#endif
