#include <QGuiApplication>
#include <QDebug>

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <iostream>

#include "operatorlayer.h"
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

  OperatorLayer operLayer(engine, ioService);

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  int returnCode = app.exec();

  threadPool.stop();
  return returnCode;
}
