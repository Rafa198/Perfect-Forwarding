#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>
#include <queue>

#include <chatmodel.h>
#include <message.h>

using namespace boost::asio;
using ip::tcp;

typedef std::deque<ChatMessage> chatMessageQueue;

class Client : public QObject, public boost::enable_shared_from_this<Client>
{
  Q_OBJECT
public:
  Client(io_service& service, const tcp::resolver::results_type& endpoints)
     : service_(service),
       socket_(service)
  {
    doConnect(endpoints);
    //socket_->connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));
  }

  Q_INVOKABLE void send1(QString user, QString mes);

  void write(const ChatMessage &msg);
  void close();

private:
  io_service& service_;
  boost::system::error_code error_;
  ChatMessage readMsg_;
  chatMessageQueue writeMsg_;
  tcp::socket socket_;

void doConnect(const tcp::resolver::results_type& endpoints);
void doReadHeader();
void doReadBody();
void doWrite();
};

#endif // CLIENT_H
