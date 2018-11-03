#include "client/client.h"

using namespace std;

int main(int argc, char *argv[]){

  if(argc < 5){
    cout << "Usage: ./client [server_ip] [port] [nickname] [channel_name]" << endl;
    exit(EXIT_SUCCESS);
  }

  TCPclient client(argv[1], atoi(argv[2]), argv[3], argv[4]);
  client.connect_serv();
  client.handler();

  return EXIT_SUCCESS;
}
