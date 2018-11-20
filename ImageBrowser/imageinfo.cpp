#include "imageinfo.h"

imageInfo::imageInfo(QObject *parent) : QObject(parent)
{
  //fileSysWatcher = new QFileSystemWatcher(this);

  //connect(fileSysWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(upd(QString)));

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

unsigned int imageInfo::getCountFilesInDir(QString dirUrl)
{
  QString str = dirUrl;
  auto filePath = QUrl(str).toLocalFile();
  QDir dir(filePath);
  return dir.count();
}

//bool imageInfo::upd(QString dirUrl, unsigned int countOld)
//{
//  unsigned int countNew = getCountFilesInDir(dirUrl);

//  if( countOld == countNew) {
//      return false;
//    } else {
//      countNew = countOld;
//      return true;
//    }
//}

//void imageInfo::upd(QString dirUrl)
//{
//  auto filePath = QUrl(dirUrl).toLocalFile();
//  fileSysWatcher->addPath(filePath);
//}
