#include "client.h"

#include <QDebug>

Client::Client(io_service &service, const tcp::resolver::results_type& endpoints)
  : service_(service)
  , socket_(service)
{
  doConnect(endpoints);
}

void Client::send1(QString user, QString mes)
{

  //qDebug() << "CLIENT: " << user << ": " << mes << endl;
  ChatMessage m((user + "|").toStdString(), mes.toStdString());
  write(m);
}

void Client::write(const ChatMessage &msg)
{
  //qDebug() << "CLIENT | WRITE!" << endl;
  boost::asio::post(service_, [this, msg]()
  {      
      //qDebug() << "CLIENT | ---POST!" << endl;
      bool writeInProgress = !writeMsg_.empty();
      writeMsg_.push_back(msg);
      
      if(!writeInProgress)
        {
          //qDebug() << "CLIENT |--- DO WRITE!" << endl;
          doWrite();
        }
  });
}

void Client::close()
{
  post(service_, [this]()
  {
    //qDebug() << "CLIENT | CLOSE" << endl;
    socket_.close();
  });
}

void Client::doConnect(const tcp::resolver::results_type& endpoints)
{
  async_connect(socket_, endpoints, [this](boost::system::error_code ec, tcp::endpoint)
  {
    if (!ec)
      {
        //qDebug() << "CLIENT| doConnect";
        doReadHeader();
      }
  });
}

void Client::doReadHeader()
{
  async_read(socket_, buffer(readMsg_.getHeader(), sizeof(ChatMessage::msgInfo)),
          [this](boost::system::error_code ec, std::size_t /*length*/)
          {
    qDebug() << "CLIENT|doReadHeader| getHeader: " << readMsg_.getHeader() << " size: " << sizeof(ChatMessage::msgInfo);
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
    qDebug() << "CLEINT|doReadBody|getBody: " << readMsg_.getBody() << " size: " << readMsg_.getBodySize();
            if (!ec)
            {
                qDebug() << QString::fromStdString(std::string(readMsg_.getBody(),readMsg_.getBodySize()));
                //layer_.add(readMsg_.getBody());
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
    qDebug() << "CLIENT|doWrite|getBuffer: " << writeMsg_.front().getBuffer()<< " size: " <<  writeMsg_.front().getSize();
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
