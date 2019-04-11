#ifndef OPERATORLAYER_H
#define OPERATORLAYER_H

#include <QQmlApplicationEngine>
#include <QObject>

#include <boost/asio.hpp>
#include "client.h"
#include "chatmodel.h"

class OperatorLayer : public QObject
{
  Q_OBJECT

public:
  OperatorLayer(QQmlApplicationEngine &engine, boost::asio::io_service &service);
  ~OperatorLayer() override;

private:
    ChatModel *m_chat_model;

    chat_client *m_client;
};

#endif // OPERATORLAYER_H
