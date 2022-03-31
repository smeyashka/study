#ifndef _CLIENT_H
#define _CLIENT_H

#include "remotemodel.h"

class Client : public RemoteModel {
public:
  Client();
  ~Client();

  void start(const char* ip_str, int port);
  
private:
  bool accept_game(const char* ip_str, int port, bool* selColor);
  
  
private:
  TCPsocket _sock;
  SDLNet_SocketSet _set_sock;
  
};


#endif
