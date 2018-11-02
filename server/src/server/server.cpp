#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <vector>

#include "server/server.h"
#include "client/client.h"

//set up the server acceptor to listen to an endpoint
server::server(const std::string host, const std::string& port)
  : _client(new client(_io)), _acceptor(_io) {

  _max_conn = 5;
  _conn = 0;
  boost::asio::ip::tcp::resolver resolver(_io);
  boost::asio::ip::tcp::resolver::query query(host, port);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
  _acceptor.open(endpoint.protocol());
  _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  _acceptor.bind(endpoint);
  _acceptor.listen();
}

//asynchronously accept a client connection and call a callback
server::recv_conn(){
  _acceptor.async_accept(_client->socket(), boost::bind(&server::handle_accept, this, boost::asio::placeholders::error));
}

//callback to start a client instance and accept more connections
server::handle_accept(const boost::system::error_code& e){

  if(e){
    std::clog << "[Error - server::handle_accept] " << e.message() << "." << std::endl;
  }else{

    _client->start();
    _client.reset(new Client(_io));
    if(_max_conn == 0 || ++_conn < _max_conn){
      _acceptor.async_accept(_client->socket(), boost::bind(&server::handle_accept, this, boost::asio::placeholders::error));
    }
  }
}
