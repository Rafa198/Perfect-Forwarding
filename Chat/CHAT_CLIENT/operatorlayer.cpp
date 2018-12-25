#include "operatorlayer.h"

#include <QQmlContext>

#include <chatmodel.h>
#include <client.h>

OperatorLayer::OperatorLayer(QQmlApplicationEngine &engine, boost::asio::io_service &service)
  : QObject()
  , chatModel_(nullptr)
  , client_(nullptr)
{
  boost::asio::ip::tcp::resolver resolver (service);
  boost::asio::ip::tcp::endpoint epoint(boost::asio::ip::address::from_string("127.0.0.1"),1234);
  auto epointI = resolver.resolve(epoint);

  client_ = new Client(service, epointI);
  chatModel_ = new ChatModel(this);

  engine.rootContext()->setContextProperty("cl", client_ );
  engine.rootContext()->setContextProperty("chmod", chatModel_);

  client_->setReadHandle(std::bind(&ChatModel::add, chatModel_, std::placeholders::_1));
}

OperatorLayer::~OperatorLayer()
{
  if (client_)
  {
    delete client_;
    client_ = nullptr;
  }
}
