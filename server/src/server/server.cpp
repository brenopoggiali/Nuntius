#include "server/server.h"

TCPserver::TCPserver(int port){

  /* create an endpoint for comunication and returns a file descriptor */
  _server_sock = socket(AF_INET, SOCK_STREAM, 0);

  /* setup the sockaddr_in struct */
  memset(&_server_addr, 0, sizeof(_server_addr));
  _server_addr.sin_family = AF_INET;
  _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  _server_addr.sin_port = htons(port);

  /* let the reuse of the port after killing the process */
  int reuse = 1;
  setsockopt(_server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));

  #ifdef SO_REUSEPORT
    setsockopt(_server_sock, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse));
  #endif

  /* binds the socket to the address and port number of the struct */
  bind(_server_sock, (struct sockaddr *)&_server_addr, sizeof(struct sockaddr));

  /* listen / waits for connections */
  listen(_server_sock, 5);

  cout << "[*] Listening on " << port << " ..." << endl;

}

void TCPserver::recv_conn(){

  while(true){

    /* set the client socket size */
    socklen_t sock_size = sizeof(_client_addr);

    /* accept the client connection */
    _client_sock = accept(_server_sock, (struct sockaddr*)&_client_addr, &sock_size);
    cout << "[*] Conection received from " << inet_ntoa(_client_addr.sin_addr) << endl;

    /* to pass more than one arg for a thread routine */
    arg_struct args;
    args.arg1 = _client_sock;
    args.arg2 = string(inet_ntoa(_client_addr.sin_addr));

    /* open a thread and assign to the client_handler routine */
    pthread_create(&_server_thread, NULL, &client_handler, (void *)&args);
  }
}

void *TCPserver::client_handler(void *a){

  arg_struct *args = (arg_struct *)a;

  int n;
  int client_sock = (long)args->arg1;
  string addr = (string)args->arg2;
  char buffer[MAXPACKETSIZE];

  /* mark the thread as detached to release its resources automatically when it terminates */
  pthread_detach(pthread_self());
  while(true){

    /* clear the buffer to receive the msg */
    memset(buffer, 0, MAXPACKETSIZE);

    /* receive the client message */
    n = recv(client_sock, buffer, MAXPACKETSIZE, 0);
    if(n < 0){

      close(client_sock);
      break;
    }
    buffer[n] = 0;
    cout << "[" << addr << "]: " << string(buffer) << endl;
  }
  return 0;
}

void TCPserver::send_msg(string msg){

  _n = send(_client_sock, msg.c_str(), msg.length(), 0);

  if(_n != msg.length()){

    cout << "ERROR fail sending message!" << endl;
    this->detach();
    exit(EXIT_FAILURE);
  }
}

void TCPserver::clean(){

  memset(_buffer, 0, MAXPACKETSIZE);
  _n = 0;
}

void TCPserver::detach(){

  close(_server_sock);
  close(_client_sock);
}
