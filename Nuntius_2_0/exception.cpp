#include "exception.h"

Exception::Exception(std::string msg) : _msg(msg) {}

const char *Exception::what() const noexcept
{
  return this->_msg.c_str();
}

SendingException::SendingException(std::string msg) : _msg(msg) {}

const char *SendingException::what() const noexcept
{
  return this->_msg.c_str();
}

ReceivingException::ReceivingException(std::string msg) : _msg(msg) {}

const char *ReceivingException::what() const noexcept
{
  return this->_msg.c_str();
}

InputException::InputException(std::string msg) : _msg(msg) {}

const char *InputException::what() const noexcept
{
  return this->_msg.c_str();
}

ServerResponseException::ServerResponseException(std::string msg) : _msg(msg) {}

const char *ServerResponseException::what() const noexcept
{
  return this->_msg.c_str();
}
