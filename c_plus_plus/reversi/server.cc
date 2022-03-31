#include <iostream>

#include "server.h"
#include "window.h"
#include "field.h"
#include "log.h"

using std::cout;
using std::cin;

Server::Server()
{
}

Server::~Server()
{
}

void Server::start(int port) 
{ 
  if (!accept_game(port)) {
    return;
  }

  cout << "Черные фишки: 0\nБелые фишки: 1\n";
  cin >> _initColor;
  bool selTurn = _initColor;
  
  send(_client, SERV, NEW, !selTurn);

  bool quit = false;
  
  SDL_Event event;
  Window w;
  Field field(Chip::ColorType(selTurn + 1));

  display(w, field);

  while (true) {
    while (SDL_PollEvent(&event)) {

      if (true == (quit = checkQuitEvent(event, _client))) {
	break;
      }

      checkVideoEvent(event, w, field);
      if (checkNewGameEvent(event, &w, &field, &selTurn)) {
	break;
      }
      checkPlacingEvent(event, _client, selTurn, &field);
    }
    if (quit == true) break;

    checkReceivedMessage(_client, _set_sock, w,  &field, &selTurn, &quit);
    if (quit == true) break;
  }

  SDLNet_TCP_DelSocket(_set_sock, _client);
  SDLNet_FreeSocketSet(_set_sock);
  SDLNet_TCP_Close(_client);
  SDLNet_Quit();  
}

IPaddress* Server::wait_connection()
{
  IPaddress* remote_ip = nullptr;
  
  //TODO maybe add an extra exit condition 
  while (1) {
    _client = SDLNet_TCP_Accept(_server);
    if (nullptr == _client) {
      SDL_Delay(100);
      continue;
    }
    
    remote_ip = SDLNet_TCP_GetPeerAddress(_client);
    if (nullptr == remote_ip) {
      errlog << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << "\n";
      continue;
    } else {
      break;
    }
  }
  
  return remote_ip;
}

bool Server::accept_game(int port)
{
  IPaddress ip;
  IPaddress* remote_ip;
  
  if (SDLNet_Init() == -1) {
    errlog << "SDLNet_Init: " << SDLNet_GetError() << "\n";
    return false;
  }

  if (SDLNet_ResolveHost(&ip, NULL, port) == -1) {
    errlog << "SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
    return false;
  }

  _server = SDLNet_TCP_Open(&ip);
  if (nullptr == _server) {
    errlog << "SDLNet_TCP_Open: " << SDLNet_GetError() << "\n";
    return false;
  }

  if (nullptr == (remote_ip = wait_connection())) {
    var(remote_ip);
    return false;    
  }
  
  Uint32 ipaddr = SDL_SwapBE32(remote_ip->host);
  cout << "Accepted a connection " << (ipaddr >> 24) << '.' << ((ipaddr >> 16) & 0xff) << '.'
       << ((ipaddr >> 8) & 0xff) << (ipaddr & 0xff) << " port " << remote_ip->port << "\n";


  _set_sock = SDLNet_AllocSocketSet(1);
  if (nullptr == _set_sock) {
    errlog << "SDLNet_AllocSocketSet:" << SDLNet_GetError() << "\n";
    return false;
  }

  if (SDLNet_TCP_AddSocket(_set_sock, _client) == -1) {
    errlog << "SDLNet_AddSocket: " << SDLNet_GetError() << "\n";
    return false;
  }

  return true;
}



// bool Server::checkQuitEvent(const SDL_Event& event)
// {
//   if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
//     send(_client, SERV, QUIT, -1);
//     dbglog << "Send quit\n";
//     return true;
//   }

//   return false;
// }

// void Server::checkVideoEvent(const SDL_Event& event, Window& w, Field& field)
// {
//   //if (event.type == SDL_VIDEORESIZE) {
//   //    w.resize(event.resize.w, event.resize.h);
//   //    display(w, field);
//   //}
  
//   if (event.type == SDL_VIDEOEXPOSE) {
//     display(w, field);
//   }  
// }

//can only server
bool Server::checkNewGameEvent(const SDL_Event& event, Window* w, Field* field, bool* selTurn)
{
  if (nullptr == field) {
    return false;
  }
  
  bool isHappend = false;
    
  //new game
  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n && event.key.keysym.mod == (KMOD_LCTRL |4096)) {
    dbglog << "Send new game\n";
    *selTurn = _initColor;
    var(*selTurn);
    send(_client, SERV, NEW, !*selTurn);
    field->reset(Chip::ColorType(*selTurn + 1));
    display(*w, *field);
    isHappend = true;
  }

  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_i && event.key.keysym.mod == (KMOD_LCTRL |4096)) {
    dbglog << "Send new inverted game\n";
    *selTurn = !*selTurn;
    var(*selTurn);
    send(_client, SERV, NEW, !*selTurn);
    field->reset(Chip::ColorType(*selTurn + 1));
    display(*w, *field);
    isHappend = true;
  }

  return isHappend;
}

// void Server::checkPlacingEvent(const SDL_Event& event, bool selTurn, Field* field)
// {
//   if (nullptr == field) {
//     return;
//   }
  
//   if (field->r_gameOver() != 2 &&
//       ((selTurn == 0 && field->currentTurn() == Chip::kBlack) ||
//        (selTurn == 1 && field->currentTurn() == Chip::kWhite))) {
    
//     try {
//       // cout<<"делаем ход\n";
//       if (field->handle_turn(event)) {
	
// 	//посылаем сообщение
// 	dbglog << "Send turn\n";
// 	send(_client, DATA, field->currentColumn(), field->currentRow());
	
// 	// cout<<"сделали\n";
// 	field->events(field->currentColumn(), field->currentRow());
	
//       } else if (field->r_gameOver() > 0) {
// 	dbglog << "Send gameOver++\n";
// 	dbglog << "Turn = " << field->currentTurn() << "\n";
// 	send(_client, SERV, RGO, -1);
//       }
//     }
    
//     catch (Field::GameOver) {
//       cout << "Game over\n";
//       field->gameOver();
//       send(_client, SERV, GO, -1);
//     }
//   }
  
// }

// void Server::checkReceivedMessage(Field* field, bool selTurn, bool* quit)
// {
//   if (nullptr == quit || nullptr == field) {
//     return;
//   }
  
//   *quit = false;
  
//   DataPacket data_paket;
//   int len = sizeof(bool)*8 + sizeof(short)*2*8;
  
//   bool new_msg = recv_mess(_set_sock, _client);
  
//   if (new_msg) {
  
//     int len_rec = SDLNet_TCP_Recv(_client, data_paket.M, len);
//     if (len_rec <= 0) {
//       errlog << "SDLNet_TCP_Recv: " << SDLNet_GetError() << "\n";
//       return;
//     }

//     dbglog << "Received: " << len_rec << " " << data_paket.M << "\n";
    
//     if (data_paket.S.type == SERV) {
//       switch (data_paket.S.data1) {
//       case QUIT:
// 	dbglog << "Quit received\n";
// 	*quit = true;
// 	break;	
//       case RGO:
// 	dbglog << "rise_gameOver received\n";
// 	field->rise_gameOver();
// 	break;
//       case GO:
// 	cout << "Game_over received\n";
// 	// selTurn = -1; 	  
// 	field->rise_gameOver();
// 	field->gameOver();
// 	break;
//       default: {
// 	errlog << "This cann't be!!!!";
//       }
//       }           
//       //new_msg = false;
//     } else {
//       if (field->r_gameOver() != 2 &&
// 	  !((selTurn == 0 && field->currentTurn() == Chip::kBlack) ||
// 	    (selTurn == 1 && field->currentTurn() == Chip::kWhite)) ) { 
// 	//получаем сообщение, проверяем на служебное
	
// 	field->events(data_paket.S.data1, data_paket.S.data2);
// 	//new_msg = false;
//       }
//     }
//   }
  
// }
