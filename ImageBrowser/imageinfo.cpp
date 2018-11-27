#include "imageinfo.h"
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <QFileInfo>

ImageInfo::ImageInfo(QObject *parent) : QObject(parent)
{


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
