#include <iostream>

#include <boost/program_options.hpp>

#include "client.h"

namespace po = boost::program_options;
using std::string;

const std::vector<string> keys = {
  "tree",
  "grass",
  "mushrooms",
  "bears",
  "fake"
};


struct Options {
  string host = "localhost";
  int port = 65500;
  string name = "client_01";
  float prob = 0.99;
  int count = 10000;
  bool verbose = false;
};



int main(int argc, char** argv)
{
  Options opt;
  
  po::options_description help {"Allowed options"};
  po::variables_map variables;

  help.add_options()
    ("help", "Show help message")
    ("host,h",  po::value<string>(&opt.host)->default_value(opt.host), "Server address")
    ("port,p",  po::value<int>(&opt.port)->default_value(opt.port),  "Server port")
    ("name,n",  po::value<string>(&opt.name)->default_value(opt.name), "Client name")
    ("count,c", po::value<int>(&opt.count)->default_value(opt.count), "Commands count")
    ("distribution,d", po::value<float>(&opt.prob)->default_value(opt.prob)
                                        ->notifier([](std::size_t value) {
					  if (value < 0 || value > 1) {
					    throw po::validation_error(po::validation_error::invalid_option_value,
								       "distribution",
								       std::to_string(value));
					  }
					}),
     "'get' command probability, range [0, 1]")
    ("verbose,v", po::bool_switch(&opt.verbose)->default_value(opt.verbose), "Verbose Command output");

  
  try {
    po::store(po::parse_command_line(argc, argv, help), variables);
    po::notify(variables);
    
    if (variables.count("help")) {
      std::cout << help;
      return 0;
    }
    
    boost::asio::io_context io_context;    
    Client client(io_context, opt.name);

    static std::streambuf* orig_buf = std::cout.rdbuf();
    if (!opt.verbose) {
      std::cout.rdbuf(nullptr);
    }
    
    client.connect(opt.host, std::to_string(opt.port));
    client.startHaos(opt.count, opt.prob, keys);
    client.disconnect();

    std::cout.rdbuf(orig_buf);
    std::cout << "Statictics: " << " \n";
    printStatistics(client);
  }

  catch (po::error& e) {
    std::cerr << "Error options! Read help, please.\n" << help;
    return -1;
  }
  
  catch (std::exception& e) {
    std::cerr << "Err: "  << e.what() << std::endl;
  }
  
  
  return 0;
}
