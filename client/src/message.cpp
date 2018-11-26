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

bool Message::is_valid_input(inputs &input, std::vector<std::string> &args)
{

  if (input == INVALID_INPUT)
  {
    return false;
  }

  switch (input)
  {
  case NICK:
    return args.size() < 1;
  }
}

void Message::handler_nick(std::string &input, std::vector<std::string> &args)
{

  std::ostringstream oss;
  oss << input << " " << args[0];
  std::string msg = oss.str();

  this->_client->send_msg(msg);
  std::string result = this->_client->recv_msg();
  inputs mapped_result = this->map_input_string(result);

  if (mapped_result != SUCCESS)
  {
    throw ServerResponseException("Fail changing nickname on server");
  }

  this->_client->set_nickname(args[0]);
}

void Message::input_handler(std::string &input)
{
  std::istringstream buf(input);
  std::istream_iterator<std::string> beg(buf), end;

  std::vector<std::string> args(beg, end);

  auto first_arg = std::move(args.front());
  std::string substr_input = first_arg.substr(1);
  args.erase(args.begin());

  inputs ipt = this->map_input_string(substr_input);

  this->_client->send_msg(start_special_handling);
  std::string ans = this->_client->recv_msg();
  inputs ans_input = this->map_input_string(ans);

  if (ans_input != SUCCESS)
  {
    throw ServerResponseException("Fail starting operation with server");
  }

  switch (ipt)
  {
  case NICK:
    // this->handler_nick(substr_input, args);
    std::cout << "FUNCAO NICK" << std::endl;
    break;
  }
}