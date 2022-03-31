#include <iostream>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <getopt.h>

#include "log.h"
#include "window.h"
#include "field.h"
#include "remotemodel.h"
#include "server.h"
#include "client.h"

using namespace std;

void display(Window& w, Field& field)
{
  field.update();
  w.update();
}

void local_game()
{  
  SDL_Event event;
  Window w;
  Field field;

  display(w, field);

  while (SDL_WaitEvent(&event) >= 0) {
    
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
      break;
    }
    //   if (event.type==SDL_VIDEORESIZE) {
    //       w.resize(event.resize.w,event.resize.h);
    //       display(w, field);
    //     }
    
    if (event.type == SDL_VIDEOEXPOSE) {
      display(w, field);
    }
  
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n && event.key.keysym.mod == (KMOD_LCTRL | 4096)) {
      //new game
      dbglog << "New game\n";
      field.reset(Chip::kNo);
      display(w, field);
    }

    if (field.r_gameOver() != 2) {
      try {
	if (field.handle_turn(event)) {
	  field.events(field.currentColumn(), field.currentRow());
	}
      }
      catch (Field::GameOver) {
	cout << "Game over\n";
	field.gameOver();
      }
    }
  }
  
}

//------------


bool recv_mess(const SDLNet_SocketSet& set_sock, const TCPsocket& sock/*, DataPacket& data*/)
{
  int num_ready = SDLNet_CheckSockets(set_sock, 100);
  if (num_ready == -1) {
    errlog << "SDLNet_CheckSocket: " << SDLNet_GetError() << "\n";
    return false;
  }
  
  if (num_ready == 0) {
    return false;
  }
  
  if (SDLNet_SocketReady(sock) == 0) {
    errlog << "It's very strange!!!\n";
    return false;
  }
  
  return true;
}


void send(const TCPsocket& sock, const TypeMes type, short data1, short data2)
{
  DataPacket data;
  data.S.type=type;
  data.S.data1=data1;
  data.S.data2=data2;
  
  unsigned int result = SDLNet_TCP_Send(sock, data.M, sizeof(data.M));
  if (result < sizeof(data.M)) {
    errlog << "SDLNet_TCP_Send: " << SDLNet_GetError() << "\n";
  }
  
  dbglog << "res=" << result << " size=" << sizeof(data.M) << endl;
}



int main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    exit(-1);
  }

  int opt = 0;
 
  static struct option long_options[] = {
    {"help",   0, 0, 'h'},
    {"server", 0, 0, 's'},
    {"client", 0, 0, 'c'},
    {0,0,0,0}
  }; 

  std::string ip;
  int port;
  int mode = 0;
    
  while (true) {
    int c = getopt_long(argc, argv, "hsci:p:", long_options, &opt);
    if (c == -1) break;
    switch (c) {
    case 'i':
      ip = optarg;
      break;
    case 'p':
      sscanf(optarg, "%d", &port);
      break;
    case 's':
      mode = 1;
      break;
    case 'c':
      mode = 2;
      break;	
    case 'h': case '?':
      cout << "Usage: " << argv[0] << " <--server|--client> -i <ip> -p <port>\n"
	"By default (whitout options) the local version is launched.\n"
	"For a client-server game you first need to start the game as a server, specifying the port to connect to. Then start the game as a client, specifying the server address and the selected port. And on the server side, you need to choose the color of the chips. To exit, press Q. The server can start a new game using C-n, or start a new game by inverting the color of the chips using C-i.\n" 
	"--server -p <port> - run server version.\n"
	"--client -i <ip> -p <port> - run client version.\n"
	"-i <ip> - server IP addres\n"
	"-p <port> - port\n"
	"-h,--help - this help\n";
      
      return 0;
    }
  }

  switch (mode) {
  case 1: {
    cout << "Start game on port " << port << ".\n";
    Server serv;
    serv.start(port);
  }
    break;
  case 2: {
    cout << "Connect to game at address " << ip << ":" << port << ".\n";
    Client client;
    client.start(ip.c_str(), port);
  }
    break;
  default:
    cout << "Start local game.\n";
    local_game();
    break;
  }
      
  cout << "Quit\n";

  SDL_Quit();

  return 0;
}

