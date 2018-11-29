#include "chatserver.h"
#include <chatsession.h>

#include <memory>

void ChatServer::do_accept()
{
  acceptor_.async_accept(socket_,
          [this](boost::system::error_code ec)
          {
            if (!ec)
            {
              std::make_shared<ChatSession>(std::move(socket_), room_)->start();
            }

            do_accept();
          });
}
