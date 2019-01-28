#include "server.h"

int main()
{
  try
  {
    boost::asio::io_service iocon;
    tcp::endpoint endpoint(tcp::v4(), 1234);
    Server server(iocon, endpoint);
    iocon.run();
  }
  catch(std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
