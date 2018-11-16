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

    //tenho q fazer uma abstracao para lancar exceptions dessas funcoes
    TCPclient *new_client = new TCPclient();

    socklen_t addr_size = sizeof(new_client->_client_addr);

    new_client->_socket = accept(this->_server_sock, (struct sockaddr *)&new_client->_client_addr, &addr_size);
    std::cout << "[*] Conection received from " << inet_ntoa(new_client->_client_addr.sin_addr) << std::endl;

    std::thread t(&TCPserver::client_handler, this, new_client);
    t.detach();
  }
}

bool TCPserver::exists_channel(std::string &name)
{
  return this->_channels.find(name) != this->_channels.end();
}

void TCPserver::add_channel(std::string &name)
{
  channel *new_channel = new channel(name);
  this->_channels.insert(std::make_pair(name, new_channel));
}

bool TCPserver::setup_client(TCPclient *client)
{

  //first receives the nickname;servername
  std::string first_msg;

  try
  {
    first_msg = client->recv_msg();
  }
  catch (const std::exception &e)
  {
    throw Exception(e.what());
  }

  int pos = first_msg.find(";");
  std::string nickname = first_msg.substr(0, pos);
  std::string channel_name = first_msg.substr(pos + 1, first_msg.length());

  if (this->exists_channel(channel_name))
  {
    if (!this->_channels.find(channel_name)->second->can_recv_client())
      return false;
    
    client->_nickname = nickname;
    client->_channel_name = channel_name;
    this->_channels.find(channel_name)->second->add_client(client);
    return true;
  }

  //channel doesn't exist
  client->_nickname = nickname;
  client->_channel_name = channel_name;
  this->add_channel(client->_channel_name);
  this->_channels.find(client->_channel_name)->second->add_client(client);
  return true;
}

void *TCPserver::client_handler(TCPclient *client)
{

  try
  {
    while (!this->setup_client(client))
    {
      //channel is full
      //depoist colocar em constantes essas mensagens de troca entre client e server
      std::string FULL = "FULL";
      client->send_msg(FULL);
    }
  }
  catch (const std::exception &e)
  {
    delete client;
    //preciso fazer um logger dpois
    std::cout << e.what() << std::endl;
    return 0;
  }

  while (true)
  {
    try
    {
      std::string msg = client->recv_msg();
      this->_channels.find(client->_channel_name)->second->send_msg(msg, client->_nickname);
    }
    catch (const std::exception &e)
    {
      auto channel = this->_channels.find(client->_channel_name);
      if (channel != this->_channels.end())
      {

        channel->second->remove_client(client->_nickname);
        int num_clients = channel->second->get_num_clients();
        if (num_clients <= 0)
        {
          delete channel->second;
          this->_channels.erase(channel);
        }
      }

      delete client;
      //preciso pensar em um melhor tratamento de errors
      std::cout << e.what() << std::endl;
      break;
    }
  }
  return 0;
}
