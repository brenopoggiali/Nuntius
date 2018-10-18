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

#define MAXPACKETSIZE 4096

using namespace std;

struct arg_struct {

  int arg1;
  string arg2;
};

class TCPserver{

private:
  int _server_sock;
  int _client_sock;
  int _n;
  int _pid;
  struct sockaddr_in _server_addr;
  struct sockaddr_in _client_addr;
  pthread_t _server_thread;
  char _buffer[MAXPACKETSIZE];

public:
  TCPserver(int port);
  void recv_conn();
  void send_msg(string msg);
  void detach();
  void clean();

  static void *client_handler(void *a);
};

#endif
