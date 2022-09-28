#include "countly/event.hpp"

namespace cly {
Event::Event(const std::string &key, size_t count) : object({}), timer_running(false) {
  object["key"] = key;
  object["count"] = count;
  setTimestamp();
}

Event::Event(const std::string &key, size_t count, double sum) : object({}), timer_running(false) {
  object["key"] = key;
  object["count"] = count;
  object["sum"] = sum;
  setTimestamp();
}

Event::Event(const std::string &key, size_t count, double sum, double duration) : object({}), timer_running(false) {
  object["key"] = key;
  object["count"] = count;
  object["sum"] = sum;
  object["dur"] = duration;

  setTimestamp();
}

Event& Event::stamp(std::chrono::system_clock::time_point timestamp)
{
  object["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count();
  return *this;
}

void Event::setTimestampOffset(std::chrono::seconds offset)
{
  timestamp_offset = offset;
}

std::chrono::system_clock::time_point Event::getTimestamp() const {
  return std::chrono::system_clock::now() - timestamp_offset;
}

void Event::setTimestamp() {
  timestamp = getTimestamp();
  object["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count();
}

void Event::startTimer() {
  setTimestamp();
  timer_running = true;
}

void Event::stopTimer() {
  if (timer_running) {
    object["dur"] = std::chrono::duration_cast<std::chrono::seconds>(getTimestamp() - timestamp).count();
    timer_running = false;
  }
}

std::string Event::serialize() const { return object.dump(); }
} // namespace cly
