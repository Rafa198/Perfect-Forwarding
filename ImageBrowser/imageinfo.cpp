#include "imageinfo.h"

imageInfo::imageInfo(QObject *parent) : QObject(parent)
{

}

QString imageInfo::getFileName(QString fileUrl)
{
  QFileInfo fi(fileUrl);
  return fi.fileName();
}

QString imageInfo::getFileLastModified(QString fileUrl)
{
  QString str1 = fileUrl;
  auto myFilePath = QUrl(str1).toLocalFile();
  QFileInfo fi(myFilePath);
  return fi.lastModified().toString("dd.MM.yyyy");;
}

QString imageInfo::getFileLastRead(QString fileUrl)
{
  QString str1 = fileUrl;
  auto myFilePath = QUrl(str1).toLocalFile();
  QFileInfo fi(myFilePath);
  return fi.lastRead().toString("dd.MM.yyyy");
}
