#include "imageinfo.h"
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>

ImageInfo::ImageInfo(QObject *parent) : QObject(parent)
{


}

QString ImageInfo::getFileName(const QString &fileUrl)
{
  QFileInfo fi(fileUrl);
  return fi.fileName();
}

QString ImageInfo::getFileLastModified(const QString &fileUrl)
{
  QFileInfo fi(fileUrl);
  return fi.lastModified().toString("dd.MM.yyyy");
}

QString ImageInfo::getFileLastRead(const QString &fileUrl)
{
  QFileInfo fi(fileUrl);
  return fi.lastRead().toString("dd.MM.yyyy");
}

qint64 ImageInfo::getFileSize(const QString &fileUrl)
{
  QFileInfo fi(fileUrl);
  return fi.size();
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
