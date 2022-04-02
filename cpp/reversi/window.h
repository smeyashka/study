#ifndef _WINDOW_HH
#define _WINDOW_HH

#include <SDL/SDL.h>
#include "text.h"

union Color_t {
  struct cc{
    unsigned int a:8;
    unsigned int b:8;
    unsigned int g:8;
    unsigned int r:8;
  } rgba;
  unsigned int col;
};

// main window
class Window {
 public:
  
  Window(int ww = 640, int hh = 670, unsigned int bc = 0x95e3a300);
  ~Window();
  
  void resize(int ww,int hh);
  void update() { if (SDL_Flip(_screen)==-1) throw Err_Init(); }

  class Err_Init {};
  
protected:
  SDL_Surface* screen() const { return _screen; }
  
private:
  
  int _width;
  int _height;
  int _bpp;

  int _old_width;
  int _old_height;

  Color_t _background;

  SDL_Surface* _screen;
};

#endif
