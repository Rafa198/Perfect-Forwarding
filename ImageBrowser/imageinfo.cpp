#include "imageinfo.h"

#include <QUrl>
#include <QDateTime>
#include <QFileInfo>

ImageInfo::ImageInfo(QObject *parent) : QObject(parent)
{


}

unsigned int ImageInfo::getCountFilesInDir(const QString &dirUrl)
{
  QString str = dirUrl;
  auto filePath = QUrl(str).toLocalFile();
  QDir dir(filePath);
  return dir.count();
}

QFileInfoList ImageInfo::getFilesPaths(const QString &fileUrl)
{
  QStringList nameFilter;
  nameFilter << "*.png" << "*.jpg";

  auto filePath = QUrl(fileUrl).toLocalFile();
  QDir dir(filePath);

  QFileInfoList list = dir.entryInfoList( nameFilter, QDir::Files );

  return list;
}
