#include "message.h"
#include "exception.h"
#include "clientdialog.h"


//preciso destruir a classe message antes da classe client caso ocorra alguma falha na conexao
Message::Message(ClientDialog *client) : _client(client)
{
  this->_mapping["INVALID_INPUT"] = INVALID_INPUT;
  this->_mapping["NICK"] = NICK;
  this->_mapping["JOIN"] = JOIN;
  this->_mapping["SUCCESS"] = SUCCESS;
  this->_mapping["FAIL"] = FAIL;
  this->_mapping["CHANNEL_IS_FULL"] = CHANNEL_IS_FULL;
  this->_mapping["NICKNAME_IN_USE"] = NICKNAME_IN_USE;
  this->_mapping["START_SPECIAL_HANDLING"] = START_SPECIAL_HANDLING;
}

Message::~Message() {}

Message::inputs Message::map_string_input(std::string &input)
{
  auto it = this->_mapping.find(input);
  if (it == this->_mapping.end())
  {
    return INVALID_INPUT;
  }
  return it->second;
}

std::string Message::map_input_string(inputs ipt)
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

bool Message::is_valid_input(inputs &input, std::vector<std::string> &args)
{
  switch (input)
  {
  case NICK:
    return args.size() == 1;
  case JOIN:
    return args.size() == 1;
  }
  return false;
}

void Message::handler_join(std::string &input, std::vector<std::string> &args)
{
  //#JOIN new_channel
  std::ostringstream oss;
  oss << input << " " << args[0];
  std::string msg = oss.str();

  this->_client->send_msg(msg);
  std::string result = this->_client->recv_msg();

  inputs mapped_result = this->map_string_input(result);

  if (mapped_result == CHANNEL_IS_FULL)
  {
    throw ServerResponseException("Channel is full");
  }
  else if (mapped_result != SUCCESS)
  {
    throw ServerResponseException("Fail joining channel on server");
  }

  this->_client->set_channel_name(args[0]);
  std::cout << "Joined successfuly! You now are on channel: " << args[0] << std::endl;
}

void Message::handler_nick(std::string &input, std::vector<std::string> &args)
{

  //#NICK new_nick
  std::ostringstream oss;
  oss << input << " " << args[0];
  std::string msg = oss.str();

  this->_client->send_msg(msg);
  std::string result = this->_client->recv_msg();

  inputs mapped_result = this->map_string_input(result);

  if (mapped_result == NICKNAME_IN_USE)
  {
    throw ServerResponseException("Nickname already in use");
  }
  else if (mapped_result != SUCCESS)
  {
    throw ServerResponseException("Fail changing nickname on server");
  }

  this->_client->set_nickname(args[0]);
  std::cout << "Nickname changed! Your new nickname is: " << args[0] << std::endl;
}

void Message::input_handler(std::string &input)
{
  std::istringstream buf(input);
  std::istream_iterator<std::string> beg(buf), end;

  std::vector<std::string> args(beg, end);

  auto first_arg = std::move(args.front());
  std::string substr_input = first_arg.substr(1);
  args.erase(args.begin());

  inputs ipt = this->map_string_input(substr_input);

  if (!this->is_valid_input(ipt, args))
  {
    throw InputException("Invalid Input");
  }

  std::string start_special_handling = this->map_input_string(START_SPECIAL_HANDLING);
  this->_client->send_msg(start_special_handling);
  std::string ans = this->_client->recv_msg();
  inputs ans_input = this->map_string_input(ans);

  if (ans_input != SUCCESS)
  {
    throw ServerResponseException("Fail starting operation with server");
  }

  switch (ipt)
  {
  case NICK:
    this->handler_nick(substr_input, args);
    break;
  case JOIN:
    this->handler_join(substr_input, args);
    break;
  }
}
