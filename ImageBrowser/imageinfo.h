#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QObject>
#include <QFileInfo>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QUrl>
#include <QDir>
#include <thread>
//#include <QFileSystemWatcher>

class imageInfo : public QObject
{
  Q_OBJECT
public:
  explicit imageInfo(QObject *parent = nullptr);
  //QFileSystemWatcher *fileSysWatcher;

  Q_INVOKABLE QString getFileName(QString fileUrl);
  Q_INVOKABLE QString getFileLastModified(QString fileUrl);
  Q_INVOKABLE QString getFileLastRead(QString fileUrl);
  unsigned int getCountFilesInDir(QString dirUrl);
  //Q_INVOKABLE bool upd(QString dirUrl, unsigned int countOld);

signals:

public slots:
//void upd(QString dirUrl);

private:

};

#endif // IMAGEINFO_H
