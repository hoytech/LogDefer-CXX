#include <sys/time.h>

#include "LogDefer.h"




static inline picojson::value render_time(struct timeval *x) {
  return picojson::value(static_cast<double>(x->tv_sec) + (static_cast<double>(x->tv_usec) / 1000000.0));
}

static inline picojson::value time_delta(struct timeval *x, struct timeval *y) {
  struct timeval delta;

  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }

  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  delta.tv_sec = x->tv_sec - y->tv_sec;
  delta.tv_usec = x->tv_usec - y->tv_usec;

  return render_time(&delta);
}





LogDefer::LogDefer(std::function<void (const std::string &)> callback) {
  callback_ = callback;

  gettimeofday(&start_tv_, NULL);

  o_.emplace("start", render_time(&start_tv_));
}

LogDefer::~LogDefer() {
  struct timeval end_tv;

  gettimeofday(&end_tv, NULL);
  o_.emplace("end", time_delta(&end_tv, &start_tv_));

  if (!data_.empty()) {
    picojson::value data_val(data_);
    o_.emplace("data", data_val);
  }

  if (!logs_.empty()) {
    picojson::value logs_val(logs_);
    o_.emplace("logs", logs_val);
  }

  if (!timers_.empty()) {
    picojson::value timers_val(timers_);
    o_.emplace("timers", timers_val);
  }

  picojson::value v(o_);
  callback_(v.serialize());
}

void LogDefer::data(const std::string &key, const std::string &value) {
  data_.emplace(key, picojson::value(value));
}

void LogDefer::add_log(int verbosity, const std::string &msg) {
  picojson::array entry;
  struct timeval end_tv;

  gettimeofday(&end_tv, NULL);

  entry.push_back(time_delta(&end_tv, &start_tv_));
  entry.push_back(picojson::value(static_cast<double>(verbosity)));
  entry.push_back(picojson::value(msg));

  picojson::value entry_val(entry);
  logs_.push_back(entry_val);
}

void LogDefer::error(const std::string &msg) {
  add_log(10, msg);
}

void LogDefer::warn(const std::string &msg) {
  add_log(20, msg);
}

void LogDefer::info(const std::string &msg) {
  add_log(30, msg);
}

void LogDefer::debug(const std::string &msg) {
  add_log(40, msg);
}
