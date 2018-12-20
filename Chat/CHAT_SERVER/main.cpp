#include "server.h"

#include <iostream>

int main()
{
  try
  {
    std::cout << "Starting..." << std::endl;
    boost::asio::io_service iocon;
    tcp::endpoint endpoint(tcp::v4(), 1234);
    chat_server server(iocon, endpoint);
    iocon.run();
  }
  catch(std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
