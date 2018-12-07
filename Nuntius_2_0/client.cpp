//#include "client.h"
//#include "message.h"

//using namespace std;

//Client::Client(std::string serv_addr, int port, std::string nickname, std::string channel_name)
//{
//  if (port < 1 || port > 65535)
//  {
//    // TODO: tenho q adicionar outra validacao para caso a porta ja esteja em uso por outro servico
//    throw Exception("Porta invalida [1 ~ 65535]");
//  }
//  this->_port = port;
//  this->_nickname = nickname;
//  this->_channel_name = channel_name;
//  this->_server_ip = serv_addr;
//  this->_message = new Message(this);

//  memset(&_server_addr, 0, sizeof(_server_addr));
//  _server_addr.sin_family = AF_INET;
//  _server_addr.sin_addr.s_addr = inet_addr(this->_server_ip.c_str());
//  _server_addr.sin_port = htons(_port);
//}

//Client::~Client()
//{
//  this->exit_server();
//}

//void Client::connect_serv()
//{

//  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
//  if (this->_socket == INVALID_SOCKET)
//  {
//    std::cout << strerror(errno) << " ERROR num: " << errno << std::endl;
//  }

//  std::cout << "[*] Trying to connect to the server at " << this->_server_ip << ":" << this->_port << std::endl;

//  int n = connect(_socket, (struct sockaddr *)&_server_addr, sizeof(_server_addr));
//  if (n == SOCKET_ERROR)
//  {
//    // TODO: melhorar mensagens de erro
//    std::cout << "[*] ERROR: Could not connect to the server" << std::endl;
//    std::cout << strerror(errno) << " ERROR num: " << errno << std::endl;
//    exit(EXIT_SUCCESS);
//  }

//  std::cout << "[*] Connection established, waiting for incoming messages..." << std::endl;
//}

//void Client::send_msg(std::string &msg)
//{
//  int n = send(this->_socket, msg.c_str(), msg.length(), 0);
//  if (n < 0)
//  {

//    this->exit_server(errno);
//    throw SendingException("Fail sending message");
//  }
//}

//std::string Client::recv_msg()
//{
//  memset(this->_buffer, 0, MAXPACKETSIZE);
//  int n = recv(this->_socket, this->_buffer, MAXPACKETSIZE, 0);

//  if (n <= 0)
//  {
//    this->exit_server(errno);
//    throw ReceivingException("Fail receiving message");
//  }
//  return std::string(this->_buffer);
//}

//void Client::connect_channel()
//{
//  std::string channel_is_full = this->_message->map_input_string(Message::CHANNEL_IS_FULL);
//  std::string nickname_in_use = this->_message->map_input_string(Message::NICKNAME_IN_USE);
//  string client_info = this->_nickname + ";" + this->_channel_name;

//  this->send_msg(client_info);
//  std::string received_msg = this->recv_msg();
//  //colocar em um enum depois
//  if (received_msg == channel_is_full)
//  {
//    throw Exception("Channel is Full");
//  }
//  if (received_msg == nickname_in_use)
//  {
//    throw Exception("Nickname already in use");
//  }
//}

//void Client::set_channel_name(std::string &channel_name)
//{
//  this->_channel_name = channel_name;
//}

//void Client::set_nickname(std::string &nickname)
//{
//  this->_nickname = nickname;
//}

//void Client::special_input_handler(std::string &input)
//{
//  try
//  {
//    this->_message->input_handler(input);
//  }
//  catch (InputException &e)
//  {
//    std::cout << e.what() << std::endl;
//  }
//  catch (ServerResponseException &e)
//  {
//    std::cout << e.what() << std::endl;
//  }
//}

//void Client::handler()
//{
//  try
//  {
//    this->connect_channel();
//  }
//  catch (Exception &e)
//  {
//    std::cout << e.what() << std::endl;
//    exit(EXIT_SUCCESS);
//  }

//  std::cout << "[*] Connected on channel " << this->_channel_name << std::endl;

//  std::thread t(&Client::msg_receiver, this);
//  t.detach();

//  std::string buffer;
//  while (getline(cin, buffer))
//  {

//    if (buffer.length() >= MAXPACKETSIZE)
//    {
//      std::cout << "ERROR message must be less than " << MAXPACKETSIZE << " bytes!" << std::endl;
//    }
//    else
//    {
//      //aq tenho q implementar sair do server dps e nao sair do programa
//      if (buffer == "#EXIT")
//      {
//        this->exit_server(-1, true);
//      }
//      else if (buffer[0] == '#')
//      {
//        this->special_input_handler(buffer);
//      }
//      else
//      {
//        this->send_msg(buffer);
//      }
//    }
//  }
//}

//void Client::exit_server(int error_code, bool should_exit)
//{
//  if (error_code != -1)
//  {
//    std::cout << strerror(error_code) << " ERROR num: " << error_code << std::endl;
//  }

//  close(this->_socket);
//  delete this->_message;
//  if (should_exit)
//  {
//    exit(EXIT_SUCCESS);
//  }
//}

//void *Client::msg_receiver()
//{

//  std::string msg;

//  while (true)
//  {
//    msg = this->recv_msg();
//    std::cout << msg << std::endl;
//  }
//  return 0;
//}
