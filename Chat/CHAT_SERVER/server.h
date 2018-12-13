#ifndef SERVER_H
#define SERVER_H

#include <boost/thread.hpp>

#include "con_handler.h"

class Server
{
public:
  Server(boost::asio::io_service &io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234))
  {
    start_accept();
  }

  void handle_accept(con_handler::pointer connection, const boost::system::error_code &err);

private:
   tcp::acceptor acceptor_;
   void start_accept();
};

#endif // SERVER_H
