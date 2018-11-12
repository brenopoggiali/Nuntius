#include "server/server.h"

TCPserver::TCPserver(int port){

  _server_sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&_server_addr, 0, sizeof(_server_addr));
  _server_addr.sin_family = AF_INET;
  _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  _server_addr.sin_port = htons(port);

  int reuse = 1;
  setsockopt(_server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));

  #ifdef SO_REUSEPORT
    setsockopt(_server_sock, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse));
  #endif

  bind(_server_sock, (struct sockaddr *)&_server_addr, sizeof(struct sockaddr));

  listen(_server_sock, 5);

  std::cout << "[*] Listening on " << port << " ..." << std::endl;

}

TCPserver::~TCPserver(){

  close(_server_sock);
}

void TCPserver::recv_conn(){

  while(true){

    auto new_client = std::make_shared<TCPclient>();

    socklen_t addr_size = sizeof(new_client->_client_addr);

    new_client->_client_sock = accept(this->_server_sock, (struct sockaddr*)&new_client->_client_addr, &addr_size);
    cout << "[*] Conection received from " << inet_ntoa(new_client->_client_addr.sin_addr) << endl;

    //aqui preciso passar nosso shared_ptr de cliente para outra thread, nao sei como posso fazer isso, pensar sobre depois...
    //alem disso preciso passar um ponteiro (normal eu acho para a classe do server para q eu possa criar os channels de dentro da thread
    //uma possivel solucao seria usar a boost/thread.h mas nao sei como funciona muito bem
    pthread_create(&_server_thread, NULL, &client_handler, (void *)&new_client);
  }
}


void *TCPserver::client_handler(void *a){

  auto *args = (arg_struct *)a;

  int n;
  int client_sock = (long)args->arg1;
  string addr = (string)args->arg2;
  char buffer[MAXPACKETSIZE];

  pthread_detach(pthread_self());
  while(true){

    memset(buffer, 0, MAXPACKETSIZE);

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
