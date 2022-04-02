#ifndef _TEXT_HH
#define _TEXT_HH

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

//! render text messages
class Text  {
public:
  class Error_text {};

  Text() {}

  ~Text() {
    if (nullptr != message) SDL_FreeSurface(message);
    if (nullptr != font) TTF_CloseFont(font);
  }

  void setColor(SDL_Color cc) { textColor = cc; }
  
  void setFont(const std::string& ff, int size) { font = TTF_OpenFont(ff.c_str(), size); if (nullptr == font) { throw Error_text(); } }
  void renderMessage(const char* mes) { message = TTF_RenderText_Solid(font, mes, textColor); }
  SDL_Surface* retMessage() { return message; }

  int w() { return message->w; }
  int h() { return message->h; }

  static void init()  { if (TTF_Init() == -1) throw Error_text(); }
  static void close() { TTF_Quit(); }

private:
  SDL_Surface* message = nullptr;
  SDL_Color textColor = {255,255,255};
  TTF_Font* font = nullptr;
 
};


#endif
