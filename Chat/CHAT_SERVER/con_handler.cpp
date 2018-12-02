#include "con_handler.h"


con_handler::con_handler(io_service &io_service): sock(io_service)
{

}

void con_handler::start()
{
  sock.async_read_some(boost::asio::buffer(data, max_length),
                       boost::bind(&con_handler::handle_read,
                       shared_from_this(),
                       boost::asio::placeholders::error,
                       boost::asio::placeholders::bytes_transferred));

  sock.async_write_some(boost::asio::buffer(message, max_length),
                        boost::bind(&con_handler::handle_write,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

void con_handler::handle_read(const boost::system::error_code& err, size_t bytes_transferred)
{
  if (!err) {
       cout << data << endl;
  } else {
       std::cerr << "ERROR: " << err.message() << std::endl;
       sock.close();
  }
}

void con_handler::handle_write(const boost::system::error_code& err, size_t bytes_transferred)
{
  if (!err)
    {
     cout << "Server sent Hello message!" << endl;
    }
  else
    {
     std::cerr << "ERROR: " << err.message() << endl;
     sock.close();
    }
}
