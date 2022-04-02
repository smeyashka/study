#include "client.h"
#include "log.h"

Client::Client()
{
}

Client::~Client()
{
}

void Client::start(const char* ip_str, int port)
{
  if (nullptr == ip_str) {
    return;
  }

  bool selColor = 1;
  
  if (!accept_game(ip_str, port, &selColor)) {
    return;
  }
  

  SDL_Event event;
  Window w;
  Field field(Chip::ColorType(selColor + 1));

  display(w, field);

  bool quit = false;

  while(true) {
    while (SDL_PollEvent(&event)) {

      if (true == (quit = checkQuitEvent(event, _sock))) {
	break;
      }
      
      checkVideoEvent(event, w, field);
      checkPlacingEvent(event, _sock, selColor, &field);      

    }
    if (quit == true) break;

    checkReceivedMessage(_sock, _set_sock, w, &field, &selColor, &quit);
    if (quit == true) break;
  }
  
  SDLNet_TCP_DelSocket(_set_sock, _sock);
  SDLNet_FreeSocketSet(_set_sock);
  SDLNet_TCP_Close(_sock);
  SDLNet_Quit();

}


bool Client::accept_game(const char* ip_str, int port, bool* selColor)
{
  DataPacket data_paket;
  int len = sizeof(bool)*8 + sizeof(short)*2*8;
  IPaddress ip;
  
  if (SDLNet_Init() == -1) {
    errlog << "SDLNet_Init: " << SDLNet_GetError() << "\n";
    return false;
  }

  if (SDLNet_ResolveHost(&ip, ip_str, port) == -1) {
    errlog << "SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
    return false;
  }

  _sock = SDLNet_TCP_Open(&ip);
  if (nullptr == _sock) {
    errlog << "SDLNet_TCP_Open: " << SDLNet_GetError() << "\n";
    return false;
  }

  _set_sock = SDLNet_AllocSocketSet(1);
  if (nullptr == _set_sock) {
    errlog << "SDLNet_AllocSocketSet:" << SDLNet_GetError() << "\n";
    return false;;
  }

  if (SDLNet_TCP_AddSocket(_set_sock, _sock) == -1) {
    errlog << "SDLNet_AddSocket: " << SDLNet_GetError() << "\n";
    return false;
  }
  
  int len_rec = SDLNet_TCP_Recv(_sock, data_paket.M, len);
  dbglog << "rec res=" << len_rec << " len=" << len << " size=" << sizeof(data_paket.M) << "\n";
  if (len_rec <= 0) {
    errlog << "SDLNet_TCP_Recv: " << SDLNet_GetError() << "\n";
    //continue;
  }

  if (data_paket.S.type != SERV || data_paket.S.data1 == QUIT) { //It's may not be
    SDLNet_TCP_Close(_sock);
    SDLNet_Quit();
    return false;
  }

  *selColor = data_paket.S.data2;
  
  return true;
}

