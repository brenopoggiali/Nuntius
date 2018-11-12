#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <map>
#include <memory>

#include "server/channel.h"
#include "server/client.h"

#define MAXPACKETSIZE 4096

typedef shared_ptr<channel> channel_ptr;

struct arg_struct {

  int arg1;
  string arg2;
};

class TCPserver{

private:
  int _server_sock;
  int _n;
  struct sockaddr_in _server_addr;
  pthread_t _server_thread;
  char _buffer[MAXPACKETSIZE];
  std::map<std::string, channel_ptr> _channels;

public:
  TCPserver(int port);
  ~TCPserver();
  void recv_conn();
  channel_ptr add_channel(std::string name);

  void* client_handler(void *a);
};

#endif
