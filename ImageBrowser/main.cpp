#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include "imageinfo.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QScopedPointer<imageInfo> imginfo(new imageInfo);

  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

   engine.rootContext()->setContextProperty("imginfo",imginfo.data());

  return app.exec();
}
