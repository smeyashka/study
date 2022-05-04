#ifndef TASK_CLIENT_H
#define TASK_CLIENT_H

#include <string>
#include <vector>
#include <random>
#include <boost/asio.hpp>


//! Statistics
struct Stat {
  int getCnt = 0;     //!< total count of 'get' commands
  int setCnt = 0;     //!< total count of 'set' commands
  int getAnswCnt = 0; //!< count answers for 'get'
  int setAnswCnt = 0; //!< count success answers for 'set'
};


class Client {
public:
  Client(boost::asio::io_context& context, const std::string& name);
  ~Client() = default;

  bool connect(const std::string& host, const std::string& port);
  bool disconnect();

  /*! start requests to the server
    \param count - total count of requests to server
    \param prob - 'get' command probability
    \param keys - list of possible keys for requests
  */
  void startHaos(int count, float prob, const std::vector<std::string>& keys);

  //! parameter request by 'key'
  std::string request(const std::string& key);

  //! set parameter by 'key' with 'value'
  std::tuple<bool, std::string> set(const std::string& key, const std::string& value);

  friend void printStatistics(const Client& cl);
  
private:

  bool reconnect();
  std::ostream& log() const;

  std::string genValue(std::default_random_engine& generator) const;
  
private:

  boost::asio::ip::tcp::socket sock_;
  boost::asio::ip::tcp::endpoint ep_;  

  std::string name_; //!< client name
  Stat stat_;        //!< statistics
};


#endif 
