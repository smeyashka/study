#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include <map>
#include <shared_mutex>
#include <condition_variable>
#include <rapidjson/document.h>


//key, <get. set>
using StatType = std::map<std::string, std::pair<ulong, ulong>>;


struct ParseException : std::runtime_error {
  ParseException(const char* msg)
    : std::runtime_error(msg) {}
};


//Parse and save config, maintance stat
class Config {
public:
  Config(const std::string& config);
  ~Config() = default;  

  std::string getAsString(const std::string& key);
  bool setFromString(const std::string& key, const std::string& val);

  bool save();

  void addStat(const std::string& key, bool isget);
  //! return current stat and reset all values
  StatType resetStat();
  
private:
  void setWriteWaiter(bool fl);
  
private:
  std::string config_;      //!< Config file name
  rapidjson::Document doc_; //!< Parsed config

  std::shared_mutex mtx_;
  std::mutex mtxCheck_;
  
  std::condition_variable docCv_; //!< waiting for document change to save it in to a file
  bool docChanged_ = false;
  std::condition_variable writeCv_; //!< readers are waiting if a writing client has appeared
  bool writeWaiter_ = false;
  
  StatType stat_; //!< current temporary statistics
};


#endif
