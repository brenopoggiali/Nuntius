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
#include <netdb.h>

using namespace std;

class TCPclient{

private:
  int _m_sock;
  int _port;
  int _n;
  string _addr;
  struct sockaddr_in _server_addr;

public:
  TCPclient(string addr, int port);
  void connect_serv();
  void handler();

};
#endif
