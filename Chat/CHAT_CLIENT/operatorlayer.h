#ifndef OPERATORLAYER_H
#define OPERATORLAYER_H
#include <QQmlApplicationEngine>
#include <QObject>

#include <boost/asio.hpp>
#include "client.h"

class OperatorLayer : public QObject
{
Q_OBJECT

public:
    OperatorLayer(QQmlApplicationEngine &engine, boost::asio::io_service &service);

    ~OperatorLayer() override;

private:
    class ChatModel* chatModel_;
    chat_client* client_;
};

#endif // OPERATORLAYER_H
