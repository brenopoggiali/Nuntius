#include "server.h"

TCPserver::TCPserver(int port)
{

  this->_mapping["INVALID_INPUT"] = INVALID_INPUT;
  this->_mapping["NICK"] = NICK;
  this->_mapping["SUCCESS"] = SUCCESS;
  this->_mapping["FAIL"] = FAIL;
  this->_mapping["START_SPECIAL_HANDLING"] = START_SPECIAL_HANDLING;

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
    User *new_client = new User();

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

bool TCPserver::setup_client(User *client)
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

TCPserver::inputs TCPserver::map_string_input(std::string &input)
{
  auto it = this->_mapping.find(input);
  if (it == this->_mapping.end())
  {
    return INVALID_INPUT;
  }
  return it->second;
}

std::string TCPserver::map_input_string(inputs ipt)
{
  for (auto it = this->_mapping.begin(); it != this->_mapping.end(); it++)
  {
    if (it->second == ipt)
    {
      return it->first;
    }
  }
  return "INVALID";
}

void TCPserver::change_user_nickname(User *client, std::string &new_nickname)
{
  auto it = this->_channels.find(client->_channel_name);
  it->second->remove_client(client->_nickname);

  client->set_nickname(new_nickname);
  it->second->add_client(client);

  std::string success = this->map_input_string(SUCCESS);
  client->send_msg(success);
}

//[TO_THINK]: aqui estou assumindo que os dados recebidos do cliente sao confiaveis e validos, pois faco a validacao no client... isso pode ser perigoso? pode dar bug?
void TCPserver::special_input_handler(User *client)
{
  std::string success = this->map_input_string(SUCCESS);
  client->send_msg(success);

  std::string input = client->recv_msg();

  std::istringstream buf(input);
  std::istream_iterator<std::string> beg(buf), end;

  std::vector<std::string> args(beg, end);

  auto first_arg = std::move(args.front());
  args.erase(args.begin());

  inputs parsed_input = this->map_string_input(first_arg);

  switch (parsed_input)
  {
  case NICK:
    this->change_user_nickname(client, args[0]);
  }
}

void *TCPserver::client_handler(User *client)
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
    std::string OK = "OK";
    client->send_msg(OK);
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
      if (msg == this->map_input_string(START_SPECIAL_HANDLING))
      {
        this->special_input_handler(client);
      }
      else
      {
        this->_channels.find(client->_channel_name)->second->send_msg(msg, client->_nickname);
      }
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
