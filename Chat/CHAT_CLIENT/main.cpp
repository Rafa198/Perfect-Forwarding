#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include "client.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  //QScopedPointer<Client> cl(new Client);
  //Client *cl = new Client();

  QGuiApplication app(argc, argv);

  //qmlRegisterType<Client>("client", 1, 0, "Client");

  QQmlApplicationEngine engine;

  //engine.rootContext()->setContextProperty("cl", cl.data());

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
