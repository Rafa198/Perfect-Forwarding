#ifndef CON_HANDLER_H
#define CON_HANDLER_H

#include <iostream>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using ip::tcp;

class con_handler : public boost::enable_shared_from_this<con_handler>
{
public:
  typedef boost::shared_ptr<con_handler> pointer;

  con_handler(boost::asio::io_service &io_service);

  void start();
  void handle_read(const boost::system::error_code &err, size_t bytes_transferred);
  void handle_write(const boost::system::error_code &err, size_t bytes_transferred);

  static pointer create(boost::asio::io_service &io_service)
  {
    return pointer(new con_handler(io_service));
  }

  tcp::socket &socket()
  {
    return sock;
  }

private:
  tcp::socket sock;
  std::string message;
  enum { max_length = 1024 };
  char data[max_length];

};
#endif // CON_HANDLER_H