#include <QGuiApplication>
#include <QDebug>


#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <iostream>

#include "client.h"
#include <ThreadPool.h>

bool exceptionHandle(const boost::exception_ptr&, const std::string&)
{
  return false;
}


int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  std::size_t cpuCount(!(boost::thread::hardware_concurrency()) ? 2 : boost::thread::hardware_concurrency() * 2);
      boost::asio::io_service ioService(cpuCount);

      std::srand(std::time(0));

      ThreadPool threadPool(ioService, cpuCount);
      threadPool.setExceptionHandler(boost::bind(&exceptionHandle, _1, _2));

      boost::asio::ip::tcp::resolver resolver (ioService);
      boost::asio::ip::tcp::endpoint epoint(boost::asio::ip::address::from_string("127.0.0.1"),1234);
      auto epointI = resolver.resolve(epoint);
      //QScopedPointer<Client> cl1(new Client(ioService,epointI));
      Client *cl1 = new Client(ioService,epointI);
      engine.rootContext()->setContextProperty("cl", cl1 );
      engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
      int returnCode = app.exec();


      threadPool.stop();


  delete cl1;
      return returnCode;
}
