#include "server.h"

int main()
{
  setlocale(LC_ALL, "Russian");
  try
  {
      boost::asio::io_service io_service;

      chat_server_list servers;
//        for (int i = 1; i < argc; ++i)
      {
          using namespace std; // For atoi.
          tcp::endpoint endpoint(tcp::v4(), 12345/*atoi(argv[i])*/);
          chat_server_ptr server(new chat_server(io_service, endpoint));
          servers.push_back(server);
      }

      io_service.run();

  }
  catch(std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
