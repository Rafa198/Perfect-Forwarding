#include "server.h"

#include <iostream>

int main()
{
  try
  {
    std::cout << "Starting..." << std::endl;
    boost::asio::io_context iocon;
    tcp::endpoint endpoint(tcp::v4(), 1234);
    chat_server server(iocon, endpoint);
    iocon.run();
    std::cout << " SUCCESS\n" << std::endl;
  }
  catch(std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
