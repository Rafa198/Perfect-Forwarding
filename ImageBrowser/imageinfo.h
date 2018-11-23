#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QObject>
#include <QString>
#include <QDir>

class ImageInfo : public QObject
{
  Q_OBJECT
public:
  explicit ImageInfo(QObject *parent = nullptr);

  Q_INVOKABLE QString getFileName(const QString &fileUrl);
  Q_INVOKABLE QString getFileLastModified(const QString &fileUrl);
  Q_INVOKABLE QString getFileLastRead(const QString &fileUrl);
  Q_INVOKABLE qint64 getFileSize(const QString &fileUrl);
  unsigned int getCountFilesInDir(const QString &dirUrl);
  QFileInfoList getFilesPaths(const QString &fileUrl);

};

#endif // IMAGEINFO_H
