#ifndef _REMOTEMODEL_H
#define _REMOTEMODEL_H

#include <iostream>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include "window.h"
#include "field.h"


enum TypeMes {
  SERV = 0,  //!< service data
  DATA = 1 //!< data
};

enum ServMes {
  NEW = 0,
  QUIT = 1,
  RGO = 2,
  GO = 3
};

union DataPacket { 
  unsigned char M[sizeof(uint8_t)*8 + sizeof(short)*2*8];
  struct { 
    TypeMes type;
    short data1; // service: 0 = new, 1 = quit, 2 = game_over++, 3 = game_over(); data: chip's column
    short data2; // service: choose chip; data: chip's row
  } S;
};

class RemoteModel {
public:

  ~RemoteModel() {}

protected:
  void display(Window& w, Field& field) const;
  
  bool messageRecieved(const SDLNet_SocketSet& set_sock, const TCPsocket& sock) const;
  void send(const TCPsocket& sock, const TypeMes type, short data1, short data2) const;

  bool checkQuitEvent(const SDL_Event& event, const TCPsocket& sock);
  void checkVideoEvent(const SDL_Event& event, Window& w, Field& field);
  void checkPlacingEvent(const SDL_Event& event, const TCPsocket& sock, bool selColor, Field* field);
  void checkReceivedMessage(TCPsocket sock, SDLNet_SocketSet set_sock, Window& w, Field* field, bool* selColor, bool* quit);
  

};

#endif
