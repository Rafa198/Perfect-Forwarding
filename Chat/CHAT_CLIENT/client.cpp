#include "client.h"

#include <QUrl>
#include <boost/filesystem.hpp>

Client::Client(io_service &service, const tcp::resolver::results_type& endpoints)
  : service_(service)
  , socket_(service)
{
  doConnect(endpoints);
}

void Client::sendMessage(QString user, QString mes)
{
  ChatMessage m((user + "|").toStdString(), mes.toStdString());
  write(m);
}

void Client::sendFile(const QString &filePath, const QString &user)
{
  sendBytes = 0;
  auto path = QUrl(filePath).toLocalFile();
  fileSize = boost::filesystem::file_size(path.toStdString());
  ifs_.open(path.toStdString(), std::ios::binary);

  if(!ifs_.is_open())
    {
      throw std::runtime_error("Unable to open input file");
    }

  long long int difference = 0;
  long long int dSize = 0;

  fileBuffer = new char[fileBufSize];


  while(sendBytes < fileSize)
    {
      difference = fileSize - sendBytes;
      dSize = fileBufSize;

      if(difference < fileBufSize)
        {
          dSize = difference;
        }

      ifs_.read(fileBuffer, dSize);
      sendBytes += dSize;
      ChatMessage mes(user.toStdString() + "|", std::string(fileBuffer, dSize), ChatMessage::Flags::FILE);
      write(mes);
    }

  ifs_.close();
  delete[] fileBuffer;
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
            handle_(readMsg_);
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
