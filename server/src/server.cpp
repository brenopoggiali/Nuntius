#include "server.h"

TCPserver::TCPserver(int port)
{

  _server_sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&_server_addr, 0, sizeof(_server_addr));
  _server_addr.sin_family = AF_INET;
  _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  _server_addr.sin_port = htons(port);

  int reuse = 1;
  setsockopt(_server_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));

#ifdef SO_REUSEPORT
  setsockopt(_server_sock, SOL_SOCKET, SO_REUSEPORT, (const char *)&reuse, sizeof(reuse));
#endif

  bind(_server_sock, (struct sockaddr *)&_server_addr, sizeof(struct sockaddr));

  listen(_server_sock, 5);

  std::cout << "[*] Listening on " << port << " ..." << std::endl;
}

TCPserver::~TCPserver()
{

  close(this->_server_sock);
}

void TCPserver::recv_conn()
{

  while (true)
  {

    TCPclient new_client = TCPclient();

    socklen_t addr_size = sizeof(new_client._client_addr);

    new_client._socket = accept(this->_server_sock, (struct sockaddr *)&new_client._client_addr, &addr_size);
    std::cout << "[*] Conection received from " << inet_ntoa(new_client._client_addr.sin_addr) << std::endl;

    std::thread t(&TCPserver::client_handler, this, std::ref(new_client));
    t.detach();
  }
}

bool TCPserver::exists_channel(std::string& name){
  return this->_channels.find(name) != this->_channels.end();
}

void TCPserver::add_channel(std::string& name){
  channel new_channel = channel(name);
  this->_channels.insert(std::make_pair(name, new_channel));
}

void *TCPserver::client_handler(TCPclient &client)
{

  //first receives the nickname;servername
  std::string first_msg;

  try
  {
    first_msg = client.recv_msg();
  }
  catch (const std::exception &e)
  {
    //preciso fazer um logger dpois
    std::cout << e.what() << std::endl;
  }

  int pos = first_msg.find(";");
  std::string nickname = first_msg.substr(0, pos);
  std::string channel_name = first_msg.substr(pos + 1, first_msg.length());

  client._nickname = nickname;
  client._channel_name = channel_name;
  if (this->exists_channel(channel_name))
  {
    if (this->_channels.find(client._channel_name)->second.can_recv_client())
    {
      this->_channels.find(client._channel_name)->second.add_client(client);
    }
    else
    {
      //channel is full
      //depoist colocar em constantes essas mensagens de troca entre client e server
      std::string FULL = "FULL";
      client.send_msg(FULL);
    }
  }
  else
  {
    //channel doesn't exist
    this->add_channel(client._channel_name);
    this->_channels.find(client._channel_name)->second.add_client(client);
  }

  while (true)
  {
    try
    {
      std::string msg = client.recv_msg();
      this->_channels.find(client._channel_name)->second.send_msg(msg, client._nickname);
    }
    catch (const std::exception &e)
    { 
      this->_channels.find(client._channel_name)->second.remove_client(client._nickname);
      //preciso pensar em um melhor tratamento de errors
      std::cout << e.what() << std::endl;
      break;
    }
  }
  return 0;
}
