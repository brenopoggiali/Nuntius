#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server/exceptions.h"

#define MAX_LENGTH 4096

class TCPclient {

  public:
    int _socket;
    int _n;
    struct sockaddr_in _client_addr;
    std::string _nickname;
    std::string _channel_name;
    char* _buffer[MAX_LENGTH];

    TCPclient();
    //caso o cliente encerre a conexao ou saia do channel preciso dar um delete nessa classe e apagar a referencia no map do channel
    //para isso preciso retornar os outputs das funcoes de conexao do cliente para q eu possa tratar esses erros na client_handler thread
    ~TCPclient();
    void send_msg(std::string& msg);
    std::string recv_msg();
    void clean();
    void detach();
};

#endif