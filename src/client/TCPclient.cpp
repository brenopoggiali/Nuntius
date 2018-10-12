#include "TCPclient.h"

using namespace std;

TCPclient::TCPclient(string addr, int port, string nickname){

  /* setup the attributes */
  _m_sock = INVALID_SOCKET;
  _addr = addr;
  _port = port;
  _nickname = nickname;
  _buffer = "";

  memset(&_server_addr, 0, sizeof(_server_addr));
  _server_addr.sin_family = AF_INET;
  _server_addr.sin_addr.s_addr = inet_addr(_addr);
  _server_addr.sin_port = htons(_port);
}

void TCPclient::connect_serv(){

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

  /* create the thread to receive messages from server */
  pthread_create(&_receiver_thread, NULL, &msg_receiver, (void *)_m_sock);

}

void *TCPclient::msg_receiver(void *a){

  int m_sock = (long)a;
  int n;
  char buff[MAXPACKETSIZE];

  /* mark the thread as detached to release its resources automatically when it terminates */
  pthread_detach(pthread_self());

  while(true){
    
    memset(buff, 0, MAXPACKETSIZE);
    n = recv(m_sock, buff, MAXPACKETSIZE, 0);
    if(n < 0){

      cout << "ERROR receiving message from server, please restart the client" << end;
      break;
    }
    cout << string(buff) << endl;
  }
}

bool TCPclient::send_msg(){

  if(_m_sock != INVALID_SOCKET){
    if(send(_m_sock, _buffer.c_str(), strlen(_buffer.c_str()), 0) >= 0){

      return true;
    }
  }
  return false
}

void TCPclient::handler(){

  while(getline(cin, _buffer)){

    //depois tenho q descontar os bytes de cabecalho no MAXPACKETSIZE (tipo: MAXP - bytes_cabecalho = new MAXP)
    if(_buffer.length() >= MAXPACKETSIZE){
      cout << "ERROR message must be less than " << MAXPACKETSIZE << " bytes!" << endl;
    }else{
      if(_buffer == "exit")
        this->detach();

      if(this->send_msg()){
        cout << _nickname << ": " << _buffer << endl;
      }else{
        this->detach("ERROR seding message to the server");
      }
    }
  }
}

void TCPclient::detach(string msg=""){

  if(msg != "")
    cout << msg << endl;

  close(_m_sock);
  exit(EXIT_SUCCESS);
}
