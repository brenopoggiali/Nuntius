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

private:
  Client *_client;
  typedef enum
  {
    INVALID_INPUT = -1,
    NICK,
    START_SPECIAL_HANDLING,
    SUCCESS,
    FAIL
  } inputs;
  std::map<std::string, inputs> _mapping;

public:
  Message(Client *client);
  ~Message();
  void input_handler(std::string &input);
  std::string map_input_string(inputs ipt);
  inputs map_string_input(std::string &input);
  void handler_nick(std::string &input, std::vector<std::string> &args);
  bool is_valid_input(inputs &input, std::vector<std::string> &args);
};

#endif