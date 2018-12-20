#include "client.h"

#include <QDebug>

void Client::send1(QString user, QString mes)
{

  qDebug() << "CLIENT: " << user << ": " << mes << endl;
  ChatMessage m;
  m.setUserName(user.toStdString());
  m.setMessage(mes.toStdString());
  m.allocate();
  m.fillBuffer();
  qDebug() << "MES SIZE: " <<  m.getMessageSize() << " Name SIZE: " << m.getUserNameSize() << endl;
  write(m);

}

void Client::write(const ChatMessage &msg)
{
  qDebug() << "CLIENT | WRITE!" << endl;
  boost::asio::post(service_, [this, msg]()
  {
      qDebug() << "CLIENT | ---POST!" << endl;
      bool writeInProgress = !writeMsg_.empty();
      writeMsg_.push_back(msg);
      if(!writeInProgress)
        {
          qDebug() << "CLIENT |--- DO WRITE!" << endl;
          doWrite();          
        }
      else
        {
         readMsg_.deallocate();
        }

  });

}

void Client::close()
{
  post(service_, [this]()
  {
    qDebug() << "CLIENT | CLOSE" << endl;
    socket_.close();
  });
}

void Client::doConnect(const tcp::resolver::results_type& endpoints)
{
  async_connect(socket_, endpoints, [this](boost::system::error_code ec, tcp::endpoint)
  {
    if (!ec)
      {
        qDebug() << "CLIENT| doConnect";
        doReadHeader();
      }
  });
}

void Client::doReadHeader()
{
  async_read(socket_, buffer(readMsg_.getMessageInfo(), sizeof(ChatMessage::msgInfo)),
          [this](boost::system::error_code ec, std::size_t /*length*/)
          {
            if (!ec)
            {
                qDebug() << "CLIENT| doReadHeader";
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
  async_read(socket_, buffer(readMsg_.getData(), readMsg_.getAllSize()),
          [this](boost::system::error_code ec, std::size_t /*length*/)
          {
            if (!ec)
            {
              readMsg_.parseBuffer();
              qDebug() << "CLIENT| doReadBody";
              //std::cout.write(readMsg_.body(), readMsg_.body_length());
              //std::cout << "\n";
              qDebug() << "UserName " + QString::fromStdString(readMsg_.getUserName()) << endl
                       << "UserNameSize " + QString(readMsg_.getUserNameSize()) << endl
                       << "Message " + QString::fromStdString(readMsg_.getMessage()) << endl
                       << "Message Size " + QString(readMsg_.getMessageSize()) << endl;
              doReadHeader();
            }
            else
            {
              socket_.close();
            }
            readMsg_.deallocate();
          });
}

void Client::doWrite()
{
  qDebug() << "CLIENT| doWrite" << endl;
  async_write(socket_, buffer(writeMsg_.front().getData(), writeMsg_.front().getAllSize()),
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
           readMsg_.deallocate();
         });

}
