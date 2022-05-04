#include "config.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>


Config::Config(const std::string& config):
  config_(config)  
{
  std::ifstream ifs(config);
  rapidjson::IStreamWrapper isw(ifs);
  
  doc_.ParseStream(isw);
  if (doc_.HasParseError()) {
    throw ParseException(rapidjson::GetParseError_En(doc_.GetParseError()));
  }
}


std::string Config::getAsString(const std::string& key)
{
  std::unique_lock lk(mtxCheck_);
  writeCv_.wait(lk, [this](){ return !writeWaiter_; });
  lk.unlock();

  std::shared_lock lock(mtx_);

  if (!doc_.HasMember(key.c_str())) {
    return std::string();
  }
 
  if (doc_[key.c_str()].GetType() == rapidjson::Type::kNumberType) {
    return std::to_string(doc_[key.c_str()].GetInt());
  }

  if (doc_[key.c_str()].GetType() == rapidjson::Type::kStringType) {
    return doc_[key.c_str()].GetString();
  }
  
  return std::string();
}


bool Config::setFromString(const std::string& key, const std::string& val)
{
  setWriteWaiter(true);

  std::unique_lock lk(mtx_);
   
  if (!doc_.HasMember(key.c_str())) {
    setWriteWaiter(false);
    writeCv_.notify_all();
    return false;
  }
  
  auto member = doc_.FindMember(key.c_str());
    
  if (doc_[key.c_str()].GetType() == rapidjson::Type::kNumberType) {
    errno = 0;
    char* p_end;
    int ival = std::strtol(val.c_str(), &p_end, 10);
    if (errno == 0 && *p_end == '\0') {
      member->value.SetInt(ival);
      docChanged_ = true;
    }
  } else {
    member->value.SetString(val.c_str(), doc_.GetAllocator());
    docChanged_ = true;
  }

  if (docChanged_) {   
    docCv_.notify_one();
  }

  setWriteWaiter(false);
  writeCv_.notify_all();
  
  return docChanged_;
}


void Config::setWriteWaiter(bool flag) {
  std::lock_guard<std::mutex> lk(mtxCheck_);
  writeWaiter_ = flag;  
}


void Config::addStat(const std::string& key, bool isget)
{
  std::lock_guard<std::mutex> lk(mtxCheck_);
  std::string curKey(key);

  if (!doc_.HasMember(curKey.c_str())) {
    curKey = std::string("mistaken");
  }
  
  if (isget) {
    ++stat_[curKey].first;
  } else {
    ++stat_[curKey].second;
  }
}


StatType Config::resetStat()
{
  std::lock_guard<std::mutex> lk(mtxCheck_);
  StatType cur;
  
  stat_.swap(cur);

  return cur;
}


bool Config::save()
{
  std::mutex mutex;
  std::unique_lock<std::mutex> cvlk(mutex);
  docCv_.wait(cvlk, [this](){ return docChanged_; });
  cvlk.unlock();

  std::shared_lock lk(mtx_);
  
  bool ok = true;
  std::ofstream ofs(config_);
  rapidjson::OStreamWrapper osw(ofs);
  rapidjson::PrettyWriter<rapidjson::OStreamWrapper> fwriter(osw);
  if (!doc_.Accept(fwriter)) {
    std::cerr << "Error write file " << config_ << "\n";
    ok = false;
  }
  
  docChanged_ = false; //not safe in common, but changed only in setFromString and guarded by mtx_
  
  return ok;
}


