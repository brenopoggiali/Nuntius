#include "../src/client/TCPclient.h"

using namespace std;

int main(int argc, char *argv[]){
  
  if(argc < 4){
    cout << "Usage: ./client [server_ip] [port] [nickname]";
    exit(EXIT_SUCCESS);
  }

  TCPclient client(argv[1], argv[2], argv[3]);
  client.connect_serv();
  client.handler();
  
  return EXIT_SUCCESS;
}