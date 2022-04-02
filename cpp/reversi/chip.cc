#include "chip.h"

Chip::Chip(int cc, int xx, int yy) :
  _color(ColorType(cc)),
  _x(xx),
  _y(yy)
{
}
  

Chip::Chip()
{
}

Chip::~Chip()
{
}

//! chip rotation animation
void Chip::showTurn(SDL_Surface* screen)
{
  Image* img = SImage::instance();
  
  SDL_Rect field;
  field.x = _x;
  field.y = _y;
  field.w = Image::kClipsWidth;
  field.h = Image::kClipsHeight;

  if (_color == kWhite) {
    for (int idx = 0; idx < Image::kClipsCount; idx++) {
      img->applySurface(_x, _y, img->turn(), screen, img->clips(idx));
      if (SDL_Flip(screen) == -1) {
	return;
      }
      SDL_Delay(70);
      if (idx != Image::kClipsCount - 1) {
	SDL_FillRect(screen, &field, 0x300060);
      }
    } 
  } else {
    for (int idx = Image::kClipsCount - 1; idx >= 0; idx--) {
      img->applySurface(_x, _y, img->turn(), screen, img->clips(idx));
      if (SDL_Flip(screen) == -1) {
	return;
      }
      SDL_Delay(70);
      if (idx != 0) {
	SDL_FillRect(screen, &field, 0x300060);
      }
    } 
  }
}

//! chip rotation
void Chip::turn()
{
  if (_color == kBlack) {
    _color = kWhite;
    _img = SImage::instance()->white();
    
  } else if (_color == kWhite){
    _color = kBlack;
    _img = SImage::instance()->black();
  }
}
