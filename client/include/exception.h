#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <stdexcept>

class Exception : public std::exception{

  std::string _msg;
  
  public:
    Exception(std::string msg);
    virtual const char* what() const noexcept override;
};

#endif