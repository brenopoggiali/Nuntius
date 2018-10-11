#include "TCPclient.h"

using namespace std;

TCPclient::TCPclient(string addr, int port){

  _m_sock = INVALID_SOCKET;
  _addr = addr;
  _port = port;

  memset(&_server_addr, 0, sizeof(_server_addr));
  _server_addr.sin_family = AF_INET;
  _server_addr.sin_addr.s_addr = inet_addr(_addr);
  _server_addr.sin_port = htons(_port);
}

TCPclient::connect_serv(){

  /* if we already have an incoming connection */
  if(_m_sock != INVALID_SOCKET){
    closesocket(_m_sock);
  }

  _m_sock = socket(AD_INET, SOCK_STREAM, 0);

  cout << "[*] Trying to connect to the server at " << _addr << ":" << _port << endl;

  _n = connect(_m_sock, (struct sockaddr *)&_server_addr, sizeof(_server_addr));
  if(_n == SOCKET_ERROR){
    cout << "[*] ERROR: Could not connect to the server" << endl;
    exit(EXIT_SUCCESS);
  }

}
