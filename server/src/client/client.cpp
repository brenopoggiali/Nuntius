#include "server/client.h"

TCPclient::TCPclient() : {}

TCPclient::~TCPclient(){
  this->detach();
};

void TCPclient::send_msg(std::string& msg){
  this->_n = send(this->_socket, msg.c_str(), msg.length(), 0);
  if(this->_n != msg.length()){

    std::cout << "ERROR fail sending message!" << std::endl;
    this->detach();
  }
}

std::string TCPclient revc_msg(){
  this->clean();
  this->_n = recv(this->_socket, this->_buffer, MAX_LENGTH, 0);
  if(this->_n < 0){
    this->detach();
    throw Exception("Fail receiving message from client");
  }
  return std::string(this->_buffer);
}

void TCPclient::detach(){
  close(this->_socket);
}

void TCPclient::clean(){
  memset(this->_buffer, 0, MAX_LENGTH);
}