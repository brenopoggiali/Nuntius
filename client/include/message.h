#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

class Client;

class Message
{

public:
  Client *_client;
  typedef enum
  {
    INVALID_INPUT = -1,
    NICK,
    JOIN,
    START_SPECIAL_HANDLING,
    SUCCESS,
    FAIL,
    NICKNAME_IN_USE,
    CHANNEL_IS_FULL
  } inputs;
  std::map<std::string, inputs> _mapping;

  Message(Client *client);
  ~Message();
  std::string map_input_string(inputs ipt);
  inputs map_string_input(std::string &input);
  bool is_valid_input(inputs &input, std::vector<std::string> &args);
  void input_handler(std::string &input);
  void handler_nick(std::string &input, std::vector<std::string> &args);
  void handler_join(std::string &input, std::vector<std::string> &args);
};

#endif