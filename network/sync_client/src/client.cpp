#include "client.h"

#include <iostream>

using boost::asio::ip::tcp;

// #define dbglog   std::cerr << __LINE__ << ": "
// #define var(v)   std::cerr << #v":" << (v) << "\n"


Client::Client(boost::asio::io_context& context, const std::string& name):
  sock_(context),
  name_(name)
{
}

std::ostream& Client::log() const
{ 
  std::ostream& out = std::cout;
  out << name_ << ": ";
  return out;
}



bool Client::connect(const std::string& host, const std::string& port)
{
  tcp::resolver resolver(sock_.get_executor());
  ::boost::asio::steady_timer timer(sock_.get_executor(), boost::asio::chrono::seconds(5));
  boost::system::error_code err;
  
  for (;;) {
    boost::asio::connect(sock_, resolver.resolve(host, port), err);
    if (!err) {
      break;
    }

    log() << "Try connect to " <<  host << ":" << port << ". Wait 3 seconds...\n";
    timer.expires_from_now(boost::asio::chrono::seconds(3));
    timer.wait();    
  }

  ep_ = sock_.remote_endpoint();
  
  log() << "connected to " << sock_.remote_endpoint() << "\n";
 
  return true;
}

bool Client::disconnect()
{
  log() << "disconnected from " << ep_ << "\n";
  
  boost::system::error_code err;
  sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
  sock_.close(err);

  return !err;
}


bool Client::reconnect()
{
  log() << "reconnect" << ep_ << "\n";

  boost::system::error_code err;
  sock_.close(err);
  connect(ep_.address().to_string(), std::to_string(ep_.port())); 

  return !err;
}


/*! start requests to the server
  \param count - total count of requests to server
  \param prob - 'get' command probability
  \param keys - list of possible keys for requests
*/
void Client::startHaos(int count, float prob, const std::vector<std::string>& keys)
{
  if (count <= 0 || keys.empty()) {
    log() << "I'm not participating today :( \n";
    return;
  }  
  
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::bernoulli_distribution genRandBool(prob);
  std::uniform_int_distribution<ushort> randKey(0, keys.size() - 1);

  bool needReconnect = false;
  
  for (int idx = 0; idx < count; idx++) {
    try {
      if (needReconnect) {
	needReconnect = false;
	reconnect();
      }
      
      std::string key = keys[randKey(generator)];
      
      if (genRandBool(generator)) {
	log() << "$get " << key << "\n";
	std::string answ = request(key);
	log() << "answer is '" << answ << "'\n";
	
      } else {
	std::string value = genValue(generator);
	log() << "$set " << key << " = " << value << "\n";
	auto [ok, answ] = set(key, value);
	log() << std::boolalpha << "set " << ok << ", answer is '" << answ << "'\n";
      }
      
    }
    catch (boost::system::system_error & err) {
      needReconnect = true;
    }        
  }

}

//generate random value with almost random length
std::string Client::genValue(std::default_random_engine& generator) const
{
  std::uniform_int_distribution<ushort> randValue(48, 122);

  uint8_t length = randValue(generator) / 25; //roughly cut and get from 1 to 4 characters
  std::string value;

  // 48-57  0-9
  // 65-90  A-Z
  // 97-122 a-z
  for (uint8_t cidx = 0; cidx < length; cidx++) {
    char ch = randValue(generator);
    //It's a little dirty, but it does the trick.
    if ( (ch > 57 && ch < 65) ||
	 (ch > 90 && ch < 97) ) {
      ch -= 7;
    }
    
    value.push_back(ch);
  }

  return value;
}

std::string Client::request(const std::string& key)
{
  std::string cmd = "$get " + key + "\n";
  boost::asio::write(sock_, boost::asio::buffer(cmd));
  ++stat_.getCnt;
  
  boost::asio::streambuf buf;
  boost::asio::read_until(sock_, buf, '\n');
  
  std::string answ((std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>());
  answ.pop_back();
  
  if (!answ.empty() && answ.find("error") == std::string::npos) {
    ++stat_.getAnswCnt;
  }
 
  return answ;
}

std::tuple<bool, std::string> Client::set(const std::string& key, const std::string& value)
{
  std::string cmd = "$set " + key + " = " + value + "\n";
  boost::asio::write(sock_, boost::asio::buffer(cmd));
  ++stat_.setCnt;

  boost::asio::streambuf buf;
  boost::asio::read_until(sock_, buf, '\n');
  
  std::string answ((std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>() );
  answ.pop_back();

  bool ok = false;  
  if (answ == key + " = " + value) {
    ++stat_.setAnswCnt;
    ok = true;
  }
 
  return {ok, answ};
}
  

void printStatistics(const Client& cl)
{
  cl.log() << "set = " << cl.stat_.setCnt << " (err = " << (cl.stat_.setCnt - cl.stat_.setAnswCnt) << ")\n";
  cl.log() << "get = " << cl.stat_.getCnt << " (err = " << (cl.stat_.getCnt - cl.stat_.getAnswCnt) << ")\n";
}
