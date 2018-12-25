#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>
#include <queue>
#include <functional>

#include <message.h>

using namespace boost::asio;
using ip::tcp;

typedef std::deque<ChatMessage> chatMessageQueue;

using ReadHandle = std::function<void(const QString&)>;

class Client : public QObject, public boost::enable_shared_from_this<Client>
{
  Q_OBJECT

public:
  Client(io_service& service, const tcp::resolver::results_type& endpoints);

  Q_INVOKABLE void send1(QString user, QString mes);

  void write(const ChatMessage &msg);
  void close();

  inline void setReadHandle(const ReadHandle& handle) { handle_ = handle; }

private:
  void doConnect(const tcp::resolver::results_type& endpoints);
  void doReadHeader();
  void doReadBody();
  void doWrite();

private:
  io_service& service_;
  boost::system::error_code error_;
  tcp::socket socket_;

  ChatMessage readMsg_;
  chatMessageQueue writeMsg_;

  ReadHandle handle_;
};

#endif // CLIENT_H
