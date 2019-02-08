#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>
#include <queue>
#include <fstream>

#include <message.h>

using namespace boost::asio;
using ip::tcp;
using ReadHandle = std::function<void(ChatMessage&)>;

typedef std::deque<ChatMessage> chatMessageQueue;

class Client : public QObject, public boost::enable_shared_from_this<Client>
{
  Q_OBJECT

public:
  Client(io_service& service, const tcp::resolver::results_type& endpoints);

  Q_INVOKABLE void sendMessage(QString user, QString mes);
  Q_INVOKABLE void sendFile(const QString &filePath, const QString &user);

  void write(const ChatMessage &msg);
  Q_INVOKABLE void close();

  inline void setReadHandle(const ReadHandle& handle) { handle_ = handle; }

//signals:
//void closeApp();

//public slots:
//  void onCloseApp();

private:
  void doConnect(const tcp::resolver::results_type& endpoints);
  void doReadHeader();
  void doReadBody();
  void doWrite();

private:
  io_service& service_;
  tcp::socket socket_;
  ChatMessage readMsg_;
  chatMessageQueue writeMsg_;
  ReadHandle handle_;
  std::ifstream ifs_;

private:
  static const unsigned int fileBufSize = 1024 * 16;
  char fileBuffer[fileBufSize];
};

#endif // CLIENT_H
