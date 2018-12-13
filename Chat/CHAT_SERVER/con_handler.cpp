#include "con_handler.h"

#include <boost/bind.hpp>

con_handler::con_handler(io_service &io_service)
  : sock(io_service)
{

}

void con_handler::start()
{
  sock.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(&data), sizeof(Message)),
                       boost::bind(&con_handler::handle_read,
                       shared_from_this(),
                       boost::asio::placeholders::error,
                       boost::asio::placeholders::bytes_transferred));

}

void con_handler::handle_read(const boost::system::error_code &err, size_t bytes_transferred)
{
  if (!err)
    {
      //std::cout << "CHAR DATA: " << data << std::endl;

      sock.async_write_some(boost::asio::buffer(reinterpret_cast<char*>(&data),bytes_transferred),
                            boost::bind(&con_handler::handle_write,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }
  else
    {
      std::cerr << "ERROR: " << err.message() << std::endl;
      sock.close();
    }
}

void con_handler::handle_write(const boost::system::error_code &err, size_t bytes_transferred)
{
  if (!err)
    {
     std::cout << "Server sent message!" << std::endl;
    }
  else
    {
     std::cerr << "ERROR: " << err.message() << std::endl;
     sock.close();
    }
}
