#include <QGuiApplication>

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cpp/operatorlayer.h>
#include <cpp/ThreadPool.h>

bool exceptionHandle(const boost::exception_ptr&, const std::string&)
{
  return false;
}

int main(int argc, char *argv[])
{
  try
  {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    size_t cpuCount(!(boost::thread::hardware_concurrency()) ? 2 : boost::thread::hardware_concurrency() * 2);
    boost::asio::io_service ioService(cpuCount);
    std::srand(std::time(nullptr));

    ThreadPool threadPool(ioService, cpuCount);
    threadPool.setExceptionHandler(boost::bind(&exceptionHandle, _1, _2));
    auto operator_layer = new OperatorLayer(engine, ioService);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    int returnCode = app.exec();
    threadPool.stop();
    delete operator_layer;
    operator_layer = nullptr;
    return returnCode;
  }
  catch (const std::exception& e)
  {
    qFatal("Unexpected error: %s", e.what());
  }
  catch (...)
  {
    qFatal("Unknown exception");
  }

}
