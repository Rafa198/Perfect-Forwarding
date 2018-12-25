#ifndef OPERATORLAYER_H
#define OPERATORLAYER_H

#include <QQmlApplicationEngine>
#include <QObject>

#include <boost/asio.hpp>

class OperatorLayer : public QObject
{
  Q_OBJECT

public:
  OperatorLayer(QQmlApplicationEngine &engine, boost::asio::io_service &service);
  ~OperatorLayer();

private:
  class ChatModel* chatModel_;
  class Client* client_;
};

#endif // OPERATORLAYER_H
