#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include <boost/asio.hpp>

#include <string>
#include <list>

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
  boost::system::error_code error;
  boost::asio::streambuf receive_buffer;
  std::list<std::string> messages;
};

#endif // CLIENT_H
