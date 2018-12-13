#include "client.h"

#include <QDebug>

#include <boost/bind.hpp>


void Client::write(const ChatMessage &msg)
{
  post(service_,
                    [this, msg]()
  {
      bool writeInProgress = !writeMsg_.empty();
      writeMsg_.push_back(msg);
      if(!writeInProgress)
        {
          doWrite();
        }
  });

}

void Client::close()
{
  post(service_, [this]()
                              {
                                socket_.close();
                              });
}

void Client::doConnect(const tcp::resolver::results_type& endpoints)
{
  async_connect(socket_, endpoints,
          [this](boost::system::error_code ec, tcp::endpoint)
          {
            if (!ec)
            {
              doReadHeader();
            }
          });
}

void Client::doReadHeader()
{
  async_read(socket_,
          buffer(readMsg_.getData(), ChatMessage::header_length),
          [this](boost::system::error_code ec, std::size_t /*length*/)
          {
            if (!ec /*&& readMsg_.decode_header()*/)
            {
              doReadBody();
            }
            else
            {
              socket_.close();
            }
          });
}

void Client::doReadBody()
{
  async_read(socket_,
          buffer(/*readmsg_.body()*/readMsg_.getData() + ChatMessage::header_length,
                              ChatMessage::max_body_length),
          [this](boost::system::error_code ec, std::size_t /*length*/)
          {
            if (!ec)
            {
              //std::cout.write(readMsg_.body(), readMsg_.body_length());
              //std::cout << "\n";
              // QDEBUG
              doReadHeader();
            }
            else
            {
              socket_.close();
            }
          });
}

void Client::doWrite()
{
  async_write(socket_,
         buffer(writeMsg_.front().getData(),
           writeMsg_.front().getAllSize()),
         [this](boost::system::error_code ec, std::size_t /*length*/)
         {
           if (!ec)
           {
             writeMsg_.pop_front();
             if (!writeMsg_.empty())
             {
               doWrite();
             }
           }
           else
           {
             socket_.close();
           }
         });
}
