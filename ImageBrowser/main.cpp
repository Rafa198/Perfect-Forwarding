#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include "imageinfo.h"
#include "testmodel.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QScopedPointer<ImageInfo> imginfo(new ImageInfo);
  QScopedPointer<TestModel> tmod(new TestModel);

  QGuiApplication app(argc, argv);

  qmlRegisterType<TestModel>("testmodel", 1, 0, "TestModel");
  QQmlApplicationEngine engine;

  engine.rootContext()->setContextProperty("imginfo", imginfo.data());
  engine.rootContext()->setContextProperty("tmod", tmod.data());
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
