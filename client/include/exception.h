#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <stdexcept>

class Exception : public std::exception
{

  std::string _msg;

public:
  Exception(std::string msg);
  virtual const char *what() const noexcept override;
};

class SendingException : public std::exception
{

  std::string _msg;

public:
  SendingException(std::string msg);
  virtual const char *what() const noexcept override;
};

class ReceivingException : public std::exception
{

  std::string _msg;

public:
  ReceivingException(std::string msg);
  virtual const char *what() const noexcept override;
};

class InputException : public std::exception
{

  std::string _msg;

public:
  InputException(std::string msg);
  virtual const char *what() const noexcept override;
};

#endif