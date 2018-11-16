#include "exception.h"

Exception::Exception(std::string msg) : _msg(msg) {}

const char* Exception::what() const noexcept {
  return this->_msg.c_str();
}