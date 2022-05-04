#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/asio/signal_set.hpp>

#include "server.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using std::string;


struct Options {
  int port = 65500;
  string config = "./config.txt";
  uint threads = 1;
};


int main(int argc, char** argv)
{
  Options opt;
  
  po::options_description help {"Allowed options"};
  po::variables_map variables;

  std::cout.precision(2);
  std::cerr.precision(2);
  help.add_options()
    ("help", "Show help message")
    ("port,p",    po::value<int>(&opt.port)->default_value(opt.port),  "Server port")
    ("config,c",  po::value<string>(&opt.config)->default_value(opt.config),  "Path to config file")
    ("threads,t", po::value<uint>(&opt.threads)->default_value(std::thread::hardware_concurrency() > 2 ?
							       std::thread::hardware_concurrency() - 2:
							       std::thread::hardware_concurrency()
							       ),
     "Number of threads to process clients");
    
  try {
    po::store(po::parse_command_line(argc, argv, help), variables);
    po::notify(variables);
    
    if (variables.count("help")) {
      std::cout << help;
      return 0;
    }
 
    boost::asio::io_context io_context;

    std::cout << "Connect to port " << opt.port << " with"  << opt.config << "\n";
    Server server(io_context, opt.port, opt.config);
    server.run(io_context, opt.threads);
        
  }
  
  catch (po::error& e) {
    std::cerr << "Error options: "  << e.what() << "\nRead help, please.\n" << help;
    return -1;
  }
  catch (ParseException& e) {
    std::cerr << "Error config file: "  << e.what() << "\n";
  }
  
  catch (...) {
    std::cerr << "Undefined error!\n";
  }
  
  return 0;
}
