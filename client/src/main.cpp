#include "client.h"

int main(int argc, char *argv[])
{

  if (argc < 4)
  {
    cout << "Usage: ./client [server_ip] [port] [nickname] [channel_name]" << endl;
    exit(EXIT_SUCCESS);
  }

  Client client(std::string(argv[1]), atoi(argv[2]), std::string(argv[3]), std::string(argv[4]));
  client.connect_serv();
  client.handler();

  return EXIT_SUCCESS;
}
