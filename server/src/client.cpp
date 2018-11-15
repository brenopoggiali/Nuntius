#include "client.h"

TCPclient::TCPclient(){
  std::cout << "cliente criado " << std::endl;
};

TCPclient::~TCPclient(){
  std::cout << "cliente destruido" << std::endl;
};

void TCPclient::send_msg(std::string& msg){
  this->_n = send(this->_socket, msg.c_str(), msg.length(), 0);
  if(this->_n != msg.length()){
    this->detach();
    throw Exception("Fail sending message to client");    
  }
}

std::string TCPclient::recv_msg(){
  this->clean();
  this->_n = recv(this->_socket, this->_buffer, MAX_LENGTH, 0);
  if(this->_n <= 0){
    std::cout << strerror(errno) << " ERROR num: " << errno << std::endl;
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