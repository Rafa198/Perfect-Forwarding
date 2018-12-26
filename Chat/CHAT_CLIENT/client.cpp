#include "client.h"

Client::Client(io_service &service, const tcp::resolver::results_type& endpoints)
  : service_(service)
  , socket_(service)
{
  doConnect(endpoints);
}

void Client::send1(QString user, QString mes)
{
  ChatMessage m((user + "|").toStdString(), mes.toStdString());
  write(m);
}

void Client::write(const ChatMessage &msg)
{
  boost::asio::post(service_, [this, msg]()
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
  async_connect(socket_, endpoints, [this](boost::system::error_code ec, tcp::endpoint)
  {
    if (!ec)
      {
        doReadHeader();
      }
  });
}

void Client::doReadHeader()
{
  async_read(socket_, buffer(readMsg_.getHeader(), sizeof(ChatMessage::msgInfo)),
          [this](boost::system::error_code ec, std::size_t /*length*/)
          {


    if (!ec)
      {
        readMsg_.deleteBuf();
        readMsg_.allocate();
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
  async_read(socket_, buffer(readMsg_.getBody(), readMsg_.getBodySize()),
          [this](boost::system::error_code ec, std::size_t /*length*/)
          {

    if (!ec)
      {
        if (handle_)
          {
            handle_(QString::fromUtf8(readMsg_.getBody(),readMsg_.getBodySize()));
          }
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
  async_write(socket_, buffer(writeMsg_.front().getBuffer(), writeMsg_.front().getSize()),
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
