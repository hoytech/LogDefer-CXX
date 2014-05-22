#ifndef LOGDEFER_H
#define LOGDEFER_H

#include "picojson/picojson.h"

class LogDefer {
  public:
    LogDefer(std::function<void (const std::string&)> callback);
    ~LogDefer();
    void add_log(int verbosity, std::string msg);
    void error(std::string msg);
    void warn(std::string msg);
    void info(std::string msg);
    void debug(std::string msg);

  private:
    std::function<void (const std::string &)> callback_;
    struct timeval start_tv_;
    picojson::object o_;
    picojson::array logs_;
    picojson::array timers_;
};

#endif
