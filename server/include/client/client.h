#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TCPclient {

  public:
    int _client_sock;
    struct sockaddr_in _client_addr;
    std::string _nickname;
    std::string _buffer;

    TCPclient();
    ~TCPclient();
    void send_msg(std::string& msg);
};

#endif