#include "client/client.h"

using namespace std;

TCPclient::TCPclient(string serv_addr, int port, string nickname)
{
  if (port < 1 || port > 65535)
  {
    //tenho q adicionar outra validacao para caso a porta ja esteja em uso por outro servico
    throw Exception("Porta invalida [1 ~ 65535]");
  }
  this->_port = port;
  this->_nickname = nickname;
  this->_serv_addr = serv_addr;

  memset(&_server_addr, 0, sizeof(_server_addr));
  _server_addr.sin_family = AF_INET;
  _server_addr.sin_addr.s_addr = inet_addr(this->_serv_addr.c_str());
  _server_addr.sin_port = htons(_port);
}

void TCPclient::connect_serv()
{

  /* if we already have an incoming connection */
  if (_socket != INVALID_SOCKET)
  {
    close(_socket);
  }

  _socket = socket(AF_INET, SOCK_STREAM, 0);

  cout << "[*] Trying to connect to the server at " << this->_serv_addr << ":" << _port << endl;

  int n = connect(_socket, (struct sockaddr *)&_server_addr, sizeof(_server_addr));
  if (n == SOCKET_ERROR)
  {
    cout << "[*] ERROR: Could not connect to the server" << endl;
    exit(EXIT_SUCCESS);
  }

  std::cout << "[*] Connection established, waiting for incoming messages..." << std::endl
            << std::endl;

  std::thread t(&TCPclient::msg_receiver, this);
  t.detach();
}

void TCPclient::send_msg(std::string &msg)
{
  int n = send(this->_socket, msg.c_str(), msg.length(), 0);
  if (n != msg.length())
  {

    this->exit();
    throw Exception("Fail sending message");
  }
}

std::string TCPclient::recv_msg()
{
  memset(this->_buffer, 0, MAXPACKETSIZE);
  int n = recv(this->_socket, this->_buffer, MAXPACKETSIZE, 0);

  if (n <= 0)
  {
    std::cout << strerror(errno) << "ERROR num: " << errno << std::endl;
    this->exit();
    throw Exception("Fail receiving message");
  }
  return std::string(this->_buffer);
}

bool TCPclient::connect_channel()
{
  std::string FULL = "FULL";
  string client_info = this->_nickname + ";" + this->_channel_name;

  try
  {
    this->send_msg(client_info);
    std::string received_msg = this->recv_msg();
    //colocar em um enum depois
    if (received_msg == FULL)
    {
      return false;
    }
    return true;
  }
  catch (std::exception &e)
  {
    throw e.what();
  }
}

void TCPclient::get_channel()
{
  std::cout << "Channel: " << std::endl;
  std::cin >> this->_channel_name;
}

void TCPclient::handler()
{

  try
  {
    while (!this->connect_channel())
    {
      std::cout << "Channel inválido!" << std::endl;
      this->get_channel();
    }
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
    this->exit();
  }

  std::string buffer;
  while (getline(cin, buffer))
  {

    if (buffer.length() >= MAXPACKETSIZE)
    {
      std::cout << "ERROR message must be less than " << MAXPACKETSIZE << " bytes!" << std::endl;
    }
    else
    {
      if (buffer == "exit")
        this->exit();

      try
      {
        this->send_msg(buffer);
      }
      catch (exception &e)
      {
        this->exit();
        std::cout << e.what() << std::endl;
        break;
      }
    }
  }
}

void TCPclient::exit(string msg)
{

  if (msg != "")
    cout << msg << endl;

  close(this->_socket);
  exit(EXIT_SUCCESS);
}

void *TCPclient::msg_receiver()
{

  std::string msg;

  while (true)
  {

    try
    {
      msg = this->recv_msg();
    }
    catch (exception &e)
    {
      std::cout << e.what() << std::endl;
      this->exit();
    }
    std::cout << msg << std::endl;
  }
  return 0;
}
