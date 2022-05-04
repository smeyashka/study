#include "server.h"
#include "config.h"

#include <iostream>

#include <boost/bind/bind.hpp>

// #define dbglog   std::cerr << __FILE__ << ':' << __LINE__ << ": "
// #define var(v)   dbglog << #v":" << (v) << "\n"

using boost::asio::ip::tcp;


Session::Session(tcp::socket socket, std::shared_ptr<Config> conf)
  : socket_(std::move(socket)),
    rx_("(\\$[gs]et)\\s*(\\w+)\\s*(?:=\\s*(\\w+))?\n"),
    conf_(conf)
{
}

//! read request from client
void Session::doRead()
{
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(read_buffer_, max_msg),
			  [this, self](boost::system::error_code ec, std::size_t length)
			  {
			    if (!ec) {
			      //dbglog << boost::this_thread::get_id() << ": read data '" << std::string(read_buffer_, length - 1) << "'\n";		      
			      std::string answer("error request\n");
			      std::cmatch cap;
			      if (std::regex_search(read_buffer_, cap, rx_)) {
				if (cap[1] == "$set") {
				  conf_->setFromString(cap[2], cap[3]);
				  conf_->addStat(cap[2], false);
				} else {
				  conf_->addStat(cap[2], true);
				}
				answer = std::string(cap[2]) + " = " + conf_->getAsString(cap[2]) + "\n";
			      }
			      
			      //dbglog << boost::this_thread::get_id() << ": " << answer;
			      std::copy(answer.begin(), answer.end(), write_buffer_);
			      
			      doWrite(answer.length());
			    }
			  });
}


//! send answer to client
void Session::doWrite(std::size_t length)
{
  auto self(shared_from_this());
  boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_, length),
			   [this, self](boost::system::error_code ec, std::size_t length)
			   {
			     if (!ec)
			       {
				 //dbglog << "write data '" << std::string(write_buffer_, length - 1) << "'\n";
				 doRead();
			       }
			   });
}


//-------------


Server::Server(boost::asio::io_context& io_context, int port, const std::string& config):
  acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
  timer_(io_context, boost::asio::chrono::seconds(5))
{
  conf_ = std::make_shared<Config>(config);
}

void Server::run(boost::asio::io_context& io_context, uint threadsCnt)
{
  //statistic
  timer_.async_wait(boost::bind(&Server::printStatistics, this));
  
  //sessions
  doAccept();

  //save doc
  threads_.create_thread([this]() { while(true) { conf_->save(); }  });
  
  for ( uint idx = 0; idx < threadsCnt; ++idx) {
    threads_.create_thread(
			   [&io_context]() {
			     io_context.run();
			   });    
  }

  io_context.run();
  
  threads_.join_all();
}


void Server::doAccept()
{
  acceptor_.async_accept(
			 [this](boost::system::error_code ec, tcp::socket socket)
			 {
			   if (!ec) {
			     std::make_shared<Session>(std::move(socket), conf_)->start();
			   }

			   doAccept();
			 });

}


void Server::printStatistics()
{
  auto print = [](auto it) {
    std::cout << " " << std::setiosflags(std::ios::left) << std::setw(10)
	      << it.first <<  std::resetiosflags(std::ios::left) << std::dec
	      << std::setw(10) << it.second.first 
              << std::setw(10) << it.second.second << "\n";
  };

  StatType cur = std::move(conf_->resetStat());
  
  std::cout << "\n*****\n"
	    << "Statistics:\n"
	    << "Last        " << std::setw(10) << "Get " << std::setw(10) << " Set\n";
  for (auto& it : cur) {
    print(it);
    stat_[it.first].first += it.second.first;
    stat_[it.first].second += it.second.second;
  }
  

  int countget = 0;
  int countset = 0;
  std::cout << "Common      " << std::setw(10) << "Get " << std::setw(10) << " Set\n";
  for (auto& it : stat_) {
    print(it);
    countget += it.second.first;
    countset += it.second.second;
  }

  std::cout << "Summary: Get = " << countget << " " 
	    << "Set = " << countset << "\n";

  std::cout << "*****\n" << std::flush;
    
  timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(5));
  timer_.async_wait(boost::bind(&Server::printStatistics, this)); 
}
			   
