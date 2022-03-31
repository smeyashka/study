#include "remotemodel.h"
#include "log.h"

void RemoteModel::display(Window& w, Field& field) const
{
  field.update();
  w.update();
}

bool RemoteModel::messageRecieved(const SDLNet_SocketSet& set_sock, const TCPsocket& sock) const
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


void RemoteModel::send(const TCPsocket& sock, const TypeMes type, short data1, short data2) const
{
  DataPacket data;
  data.S.type=type;
  data.S.data1=data1;
  data.S.data2=data2;
  
  unsigned int result = SDLNet_TCP_Send(sock, data.M, sizeof(data.M));
  if (result < sizeof(data.M)) {
    errlog << "SDLNet_TCP_Send: " << SDLNet_GetError() << "\n";
  }
  
  errlog << "res=" << result << " size=" << sizeof(data.M) << "\n";
}


bool RemoteModel::checkQuitEvent(const SDL_Event& event, const TCPsocket& sock)
{
  if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
    send(sock, SERV, QUIT, -1);
    dbglog << "Send quit\n";
    return true;
  }

  return false;
}


void RemoteModel::checkVideoEvent(const SDL_Event& event, Window& w, Field& field)
{
  //if (event.type == SDL_VIDEORESIZE) {
  //    w.resize(event.resize.w, event.resize.h);
  //    display(w, field);
  //}
  
  if (event.type == SDL_VIDEOEXPOSE) {
    display(w, field);
  }  
}


void RemoteModel::checkPlacingEvent(const SDL_Event& event, const TCPsocket& sock, bool selColor, Field* field)
{
  if (nullptr == field) {
    return;
  }
  
  if (field->r_gameOver() != 2 &&
      ((selColor == 0 && field->currentTurn() == Chip::kBlack) ||
       (selColor == 1 && field->currentTurn() == Chip::kWhite))) {
    
    try {
      // cout<<"делаем ход\n";
      if (field->handle_turn(event)) {
	
	//посылаем сообщение
	dbglog << "Send turn\n";
	send(sock, DATA, field->currentColumn(), field->currentRow());
	
	// cout<<"сделали\n";
	field->events(field->currentColumn(), field->currentRow());
	
      } else if (field->r_gameOver() > 0) {
	dbglog << "Send gameOver++\n";
	dbglog << "Turn = " << field->currentTurn() << "\n";
	send(sock, SERV, RGO, -1);
      }
    }
    
    catch (Field::GameOver) {
      std::cout << "Game over\n";
      field->gameOver();
      send(sock, SERV, GO, -1);
    }
  }
  
}


void RemoteModel::checkReceivedMessage(TCPsocket sock, SDLNet_SocketSet set_sock, Window& w, Field* field, bool* selColor, bool* quit)
{
  if (nullptr == quit || nullptr == field) {
    return;
  }
  
  *quit = false;
  
  DataPacket data_paket;
  int len = sizeof(bool)*8 + sizeof(short)*2*8;
  
  bool new_msg = messageRecieved(set_sock, sock);
  
  if (new_msg) {
  
    int len_rec = SDLNet_TCP_Recv(sock, data_paket.M, len);
    if (len_rec <= 0) {
      errlog << "SDLNet_TCP_Recv: " << SDLNet_GetError() << "\n";
      return;
    }

    dbglog << "Received: " << len_rec << " " << data_paket.M << "\n";
    
    if (data_paket.S.type == SERV) {
      switch (data_paket.S.data1) {

      case NEW:
	dbglog << "New game received\n";
	*selColor = data_paket.S.data2;
	var(*selColor);
	field->reset(Chip::ColorType(*selColor + 1));
	display(w, *field);
      	break;
      case QUIT:
	dbglog << "Quit received\n";
	*quit = true;
	break;	
      case RGO:
	dbglog << "rise_gameOver received\n";
	field->rise_gameOver();
	break;
      case GO:
	dbglog << "Game_over received\n";
	field->rise_gameOver();
	field->gameOver();
	break;
      default: {
	errlog << "This cann't be!!!!";
      }
      }           
	
    } else {
      if (field->r_gameOver() != 2 &&
	  !((*selColor == 0 && field->currentTurn() == Chip::kBlack) ||
	    (*selColor == 1 && field->currentTurn() == Chip::kWhite)) ) { 
	//received message, check if service
	field->events(data_paket.S.data1, data_paket.S.data2);
      }
    }
  }
  
}
