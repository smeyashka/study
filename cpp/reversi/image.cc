#include "image.h"
#include "log.h"

#include <SDL/SDL.h>

#include <iostream>

using std::string;

template<> Image* SImage::_instance = 0;

#define BLACK_FISHKA_IMG_FILE  "./resources/black1.png"
#define WHITE_FISHKA_IMG_FILE  "./resources/white1.png"
#define TURN_FISHKA_IMG_FILE   "./resources/fishki_turn_new1.png"
#define GRID_IMG_FILE          "./resources/grid1.png"

Image::Image()
{
  if (!_init) {
    _init = loadFiles();
    setClips();
    _init = true;
  }
}


Image::~Image()
{
  if (_init) {
    if (_black != nullptr) SDL_FreeSurface(_black);
    if (_white != nullptr) SDL_FreeSurface(_white); 
    if (_turn  != nullptr) SDL_FreeSurface(_turn);
    if (_grid  != nullptr) SDL_FreeSurface(_grid);
    _init = false;
  }
}
  

void Image::setClips()
{
  for (int idx = 0; idx < 4; idx++) {
    _clips[idx].x = kClipsWidth * idx;
    _clips[idx].y = 0;
    _clips[idx].w = kClipsWidth;
    _clips[idx].h = kClipsHeight;
  }
  
  for (int idx = 0; idx < 3; idx++) {
    _clips[idx + 4].x = kClipsWidth * idx;
    _clips[idx + 4].y = kClipsHeight;
    _clips[idx + 4].w = kClipsWidth;
    _clips[idx + 4].h = kClipsHeight;
  }
}

bool Image::loadFiles()
{
  _black = loadImage(BLACK_FISHKA_IMG_FILE);
  _white = loadImage(WHITE_FISHKA_IMG_FILE);
  _turn = loadImage(TURN_FISHKA_IMG_FILE);
  _grid = loadImage(GRID_IMG_FILE);
  
  if (_black == nullptr || _white == nullptr ||
      _turn  == nullptr || _grid  == nullptr) {
    return false;
  }

  return true;
}

SDL_Surface* Image::loadImage(std::string filename)
{
  SDL_Surface* loadedImage = nullptr;
  SDL_Surface* optimizedImage = nullptr;

  loadedImage = IMG_Load(filename.c_str());
  
  if (loadedImage != nullptr) {
    optimizedImage = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
    
    if (optimizedImage != nullptr) {
     Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0x30, 0x00, 0x60);  
     SDL_SetColorKey(optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
    } 
  }
  
  return optimizedImage;
}

void Image::applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip /*= nullptr*/)
{
  if (source == nullptr || destination == nullptr) {
    return;
  }
  
  SDL_Rect offset;
  offset.x=x;
  offset.y=y;
  
  SDL_BlitSurface(source, clip, destination, &offset);
}

