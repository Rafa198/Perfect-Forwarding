#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <boost/asio.hpp>

#include <chatroom.h>

using boost::asio::ip::tcp;

class ChatServer
{
public:
 ChatServer(boost::asio::io_service& io_service,
        const tcp::endpoint& endpoint)
      : acceptor_(io_service, endpoint),
        socket_(io_service)
    {
      do_accept();
    }


private:
  void do_accept();
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  ChatRoom room_;
};

#endif // CHATSERVER_H
