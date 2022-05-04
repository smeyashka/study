#ifndef TASK_SERVER_H
#define TASK_SERVER_H

#include <string>
#include <regex>


#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "config.h"


class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(boost::asio::ip::tcp::socket socket, std::shared_ptr<Config> conf);
  ~Session() = default;

  void start() { doRead(); }

private:
  void doRead();
  void doWrite(std::size_t length);

private:
  boost::asio::ip::tcp::socket socket_;
  std::regex rx_;

  std::shared_ptr<Config> conf_;
  
  enum { max_msg = 1024 };
  char read_buffer_[max_msg];
  char write_buffer_[max_msg];

};



class Server {
public:
  Server(boost::asio::io_context& io_context, int port, const std::string& config);
  ~Server() = default;

  void run(boost::asio::io_context& io_context, uint threadsCnt);
  void printStatistics();
  
private:
  
  void doAccept();
  
private:
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::steady_timer timer_;
  
  boost::thread_group threads_;
  
  std::shared_ptr<Config> conf_;
  StatType stat_;
};



#endif
