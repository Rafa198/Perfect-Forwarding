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

  QFileInfoList getFilesPaths(const QString &fileUrl);

};

#endif // IMAGEINFO_H
