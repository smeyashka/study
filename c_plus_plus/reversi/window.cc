#include "window.h"

Window::Window(int ww, int hh, unsigned int bc):
  _width(ww),
  _height(hh),
  _bpp(32),
  _old_width(ww),
  _old_height(hh)
{
  _background.col=bc;

  _screen = SDL_SetVideoMode(_width, _height, _bpp, SDL_SWSURFACE /*| SDL_OPENGL *//*| SDL_RESIZABLE*/);
  if (_screen == nullptr) {
    throw Err_Init();
  }

  SDL_WM_SetCaption("Reversi", NULL);

  Text::init();
}

Window::~Window() {
  Text::close();
}

void Window::resize(int ww, int hh)
{
  _width = ww;
  _height = hh;

  _screen = SDL_SetVideoMode(_width, _height, _bpp, SDL_SWSURFACE/* | SDL_OPENGL*/ | SDL_RESIZABLE);
  if (_screen == nullptr) {
    throw Err_Init();
  }
}
