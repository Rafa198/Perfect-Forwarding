#include "imageinfo.h"
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <QFileInfo>

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
  QString str1 = fileUrl;
  auto myFilePath = QUrl(str1).toLocalFile();
  QFileInfo fi(myFilePath);
  return fi.lastModified().toString("dd.MM.yyyy");;
}

QString ImageInfo::getFileLastRead(const QString &fileUrl)
{
  QString str1 = fileUrl;
  auto myFilePath = QUrl(str1).toLocalFile();
  QFileInfo fi(myFilePath);
  return fi.lastRead().toString("dd.MM.yyyy");
}

unsigned int ImageInfo::getCountFilesInDir(const QString &dirUrl)
{
  QString str = dirUrl;
  auto filePath = QUrl(str).toLocalFile();
  QDir dir(filePath);
  return dir.count();
}
