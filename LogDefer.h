#pragma once

#include "picojson/picojson.h"

class LogDefer {
  public:
    LogDefer(std::function<void (const std::string&)> callback);
    ~LogDefer();
    void add_log(int verbosity, const std::string &msg);
    void error(const std::string &msg);
    void warn(const std::string &msg);
    void info(const std::string &msg);
    void debug(const std::string &msg);

  private:
    std::function<void (const std::string &)> callback_;
    struct timeval start_tv_;
    picojson::object o_;
    picojson::array logs_;
    picojson::array timers_;
};
