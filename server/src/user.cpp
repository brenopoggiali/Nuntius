#include "user.h"

User::User()
{
  std::cout << "user criado " << std::endl;
};

User::~User()
{
  std::cout << "user destruido" << std::endl;
};

void User::send_msg(std::string &msg)
{
  this->_n = send(this->_socket, msg.c_str(), msg.length(), 0);
  if (this->_n != msg.length())
  {
    this->detach();
    throw Exception("Fail sending message to client");
  }
}

std::string User::recv_msg()
{
  this->clean();
  this->_n = recv(this->_socket, this->_buffer, MAX_LENGTH, 0);
  if (this->_n <= 0)
  {
    std::cout << strerror(errno) << " ERROR num: " << errno << std::endl;
    this->detach();
    throw Exception("Fail receiving message from client");
  }
  return std::string(this->_buffer);
}

void User::detach()
{
  close(this->_socket);
}

void User::clean()
{
  memset(this->_buffer, 0, MAX_LENGTH);
}

void User::set_nickname(std::string nickname)
{
  this->_nickname = nickname;
}