#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include <boost/asio.hpp>

#include <string>

using namespace boost::asio;
using ip::tcp;

class Client : public QObject
{
  Q_OBJECT
public:
  explicit Client(QObject *parent = nullptr)
   : QObject(parent)
  {}

  Q_INVOKABLE void client_run(QString);

private:
  boost::asio::io_service service;
  //std::string msg = "-FROM CLIENT!\n";
  boost::system::error_code error;
  boost::asio::streambuf receive_buffer;
};

#endif // CLIENT_H
