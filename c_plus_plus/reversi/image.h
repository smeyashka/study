#ifndef _IMAGE_HH
#define _IMAGE_HH

#include "singleton.h"

#include <SDL/SDL_image.h>
#include <string>
#include <iostream>

class SDL_Surface;
class SDL_Rect;


//! load all resource (images for field and chips)
class Image {
public:
  static const uint8_t kClipsWidth = 75;
  static const uint8_t kClipsHeight = 75;
  static const uint8_t kClipsCount = 7;
  
  Image();
  ~Image();
  
  static SDL_Surface* loadImage(std::string filename);
  void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = nullptr);
  
  SDL_Surface* black() { return _black; }
  SDL_Surface* white() { return _white; }
  SDL_Surface* turn()  { return _turn; }
  SDL_Surface* grid()  { return _grid; }
  
  SDL_Rect* clips(int idx) {
    if (idx >= kClipsCount) return nullptr;
    return &(_clips[idx]);
  }
  
private:
  bool loadFiles();
  void setClips();
  
private:
  SDL_Surface* _black = nullptr; //!< black chip image
  SDL_Surface* _white = nullptr; //!< white chip image
  SDL_Surface* _turn  = nullptr; //!< rotating chip image
  SDL_Surface* _grid  = nullptr; //!< grid image
  
  SDL_Rect _clips[kClipsCount];  //!< coordinates of sections rotating chips
  
  bool _init = false;
};


typedef Singleton<Image> SImage;


#endif
