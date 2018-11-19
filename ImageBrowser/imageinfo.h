#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QObject>
#include <QFileInfo>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QUrl>
#include <QDir>

class imageInfo : public QObject
{
  Q_OBJECT
public:
  explicit imageInfo(QObject *parent = nullptr);
  Q_INVOKABLE QString getFileName(QString fileUrl);
  Q_INVOKABLE QString getFileLastModified(QString fileUrl);
  Q_INVOKABLE QString getFileLastRead(QString fileUrl);
signals:

public slots:
};

#endif // IMAGEINFO_H
