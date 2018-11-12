#include "server/client.h"

TCPclient::TCPclient() : {}

TCPclient::~TCPclient(){
  this->detach();
};

void TCPclient::send_msg(std::string& msg){
  this->_n = send(this->_client_sock, msg.c_str(), msg.length(), 0);
  if(this->_n != msg.length()){

    std::cout << "ERROR fail sending message!" << std::endl;
    this->detach();
  }
}

void TCPclient::detach(){
  close(this->_client_sock);
}