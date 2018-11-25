#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

std::string start_special_handling = "START_SPECIAL_HANDLING";

class Client;

class Message
{

private:
  Client *_client;
  typedef enum
  {
    INVALID_INPUT = -1,
    NICK,
    SUCCESS,
    FAIL
  } inputs;
  std::map<std::string, inputs> _mapping;

public:
  Message(Client *client);
  ~Message();
  void input_handler(std::string &input, std::vector<string>);
  inputs map_input_string(std::string &input);
  void handler_nick(std::string &input, std::vector<std::string> &args);
};

#endif