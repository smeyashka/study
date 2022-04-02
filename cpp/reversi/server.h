#ifndef _SERVER_H
#define _SERVER_H

#include "remotemodel.h"

class Server : public RemoteModel {
public:
  Server();
  ~Server();

  void start(int port);
  
private:
  bool accept_game(int port);
  IPaddress* wait_connection();
  
  //bool checkQuitEvent(const SDL_Event& event);
  //void checkVideoEvent(const SDL_Event& event, Window& w, Field& field);
  bool checkNewGameEvent(const SDL_Event& event, Window* w, Field* field, bool* selChip);
  void checkTurnEvent(const SDL_Event& event, bool selChip, Field* field, bool* new_msg);
  //void checkPlacingEvent(const SDL_Event& event, bool selChip, Field* field);
  //void checkReceivedMessage(Field* field, bool selChip, bool* quit);
  
private:
  TCPsocket _server;
  TCPsocket _client;
  SDLNet_SocketSet _set_sock;
  bool _initColor = 0; //bool - for convient inversion, 0 - black, 1 - white
  
};

#endif
