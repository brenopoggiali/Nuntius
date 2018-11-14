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

//dps fazer enum
#define MAXPACKETSIZE 4096
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

using namespace std;

class TCPclient{

private:
  int _m_sock;
  int _port;
  int _n;
  string _addr;
  string _nickname;
  string _buffer;
  string _channel_name;
  struct sockaddr_in _server_addr;
  pthread_t _receiver_thread;

public:
  TCPclient(string addr, int port, string nickname, string channel_name);
  void connect_serv();
  void handler();
  void detach(string msg="");
  bool send_msg();

  static void *msg_receiver(void *a);

};
#endif
