#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>

#include "exception.h"

//dps fazer enum
#define MAXPACKETSIZE 4096
#define SOCKET_ERROR -1

using namespace std;

class TCPclient{

private:
  int _socket;
  int _port;
  std::string _serv_addr;
  string _nickname;
  char _buffer[MAXPACKETSIZE];
  string _channel_name;
  struct sockaddr_in _server_addr;

public:
  TCPclient(std::string addr, int port, std::string nickname);
  void connect_serv();
  void handler();
  void exit(std::string msg="");
  void send_msg(std::string& msg);
  std::string recv_msg();
  bool connect_channel();
  void get_channel();

  void *msg_receiver();

};
#endif
