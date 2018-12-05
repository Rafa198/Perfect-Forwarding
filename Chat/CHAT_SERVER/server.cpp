#include "server.h"

void Server::handle_accept(con_handler::pointer connection, const boost::system::error_code &err)
{
  if (!err)
    {
      connection->start();
    }
  start_accept();
}
void Server::start_accept()
{
 // socket
  con_handler::pointer connection = con_handler::create(acceptor_.get_io_service());

 // asynchronous accept operation and wait for a new connection.
  acceptor_.async_accept(connection->socket(),
                         boost::bind(&Server::handle_accept,
                         this,
                         connection,
                         boost::asio::placeholders::error));
}
