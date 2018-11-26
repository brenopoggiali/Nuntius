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
#include <map>
#include <memory>
#include <thread>

#include "channel.h"
#include "user.h"
#include "exception.h"

#define MAX_LENGTH 4096

class TCPserver
{

private:
  int _server_sock;
  int _n;
  struct sockaddr_in _server_addr;
  std::map<std::string, channel *> _channels;

public:
  TCPserver(int port);
  ~TCPserver();
  void recv_conn();
  bool exists_channel(std::string &name);
  void add_channel(std::string &name);
  bool setup_client(User *client);

  void *client_handler(User *client);
};

#endif
