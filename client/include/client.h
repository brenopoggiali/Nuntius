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
#include <errno.h>
#include <vector>

#include "exception.h"

//dps fazer enum
#define MAXPACKETSIZE 4096
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

using namespace std;

class Message;

class Client
{

private:
  int _socket;
  int _port;
  std::string _server_ip;
  std::string _nickname;
  char _buffer[MAXPACKETSIZE];
  std::string _channel_name;
  struct sockaddr_in _server_addr;
  Message *_message;

public:
  Client(std::string addr, int port, std::string nickname, std::string channel_name);
  ~Client();
  void connect_serv();
  void handler();
  void exit_server(int error_code = -1, bool should_exit = false);
  void send_msg(std::string &msg);
  std::string recv_msg();
  void connect_channel();
  void set_nickname(std::string &nickname);
  void set_channel_name(std::string &channel_name);
  void special_input_handler(std::string &input);

  void *msg_receiver();
};
#endif
