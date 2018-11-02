#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

class client;
typedef boost::shared_ptr<client> client_ptr;

class server : private boost::noncopyable {

private:
  boost::asio::io_service _io;
  boost::asio::ip::tcp::acceptor _acceptor;
  client_ptr _client;
  int _conn, _max_conn;

  void handle_accept(const boost::system::error_code& e);

public:
  explicit server(const std::string host, const std::string& port);
  void recv_conn();

};

#endif
