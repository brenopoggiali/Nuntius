#include "message.h"
#include "exception.h"
#include "client.h"

//preciso destruir a classe message antes da classe client caso ocorra alguma falha na conexao
Message::Message(Client *client) : _client(client) {}

Message::~Message() {}

Message::inputs Message::map_input_string(std::string &input)
{
  auto it = this->_mapping.find(input);
  if (it == this->_mapping.end())
  {
    return INVALID_INPUT;
  }
  return it->second;
}

void Message::handler_nick(std::string &input, std::vector<std::string> &args)
{
  if (args.size() < 1)
  {
    throw InputException("Invalid arguments");
  }

  std::ostringstream oss;
  oss << input << " " << args[0];
  std::string msg = oss.str();

  this->_client->send_msg(msg);
  std::string result = this->_client->recv_msg();
  inputs mapped_result = this->map_input_string(result);

  if (mapped_result != SUCCESS)
  {
    throw Exception("");
  }

  this->_client->set_nickname(args[0]);
}

void Message::input_handler(std::string &input, std::vector<std::string> args)
{

  //converte string input to enum input
  std::string substr_input = input.substr(1);
  inputs ipt = this->map_input_string(substr_input);
  if (ipt == INVALID_INPUT)
  {
    throw InputException("Inalid input");
  }

  //tells the server about our special handlig
  this->_client->send_msg(start_special_handling);
  std::string ans = this->_client->recv_msg();
  inputs ans_input = this->map_input_string(ans);

  if (ans_input != SUCCESS)
  {
    throw Exception("Fail starting operation with server");
  }

  try
  {
    switch (ipt)
    {
    case NICK:
      this->handler_nick(substr_input, args);
      break;
    }
  }
  catch (std::exception &e)
  {
    throw Exception("Erro na funcao handler");
  }
}