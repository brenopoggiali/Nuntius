#include "../src/server/TCPserver.h"

using namespace std;

int main(int argc, char *argv[]){

  if(argc < 2){
    cout << "Usage: ./server [port]" << endl;
    exit(EXIT_SUCCESS);
  }

  TCPserver serv(atoi(argv[1]));
  serv.recv_conn();

  return EXIT_SUCCESS;
}
