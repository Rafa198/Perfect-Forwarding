#include "testmodel.h"
#include <QDebug>
#include <QUrl>
#include <QDateTime>
#include <thread>
#include <functional>

TestModel::TestModel(QObject *parent):
    QAbstractListModel(parent)
{
  qRegisterMetaType<QFileInfo>("QFileInfo");
  QObject::connect(this,SIGNAL(changed(QFileInfo)),this, SLOT(insertInList(QFileInfo)),Qt::QueuedConnection);
}

int TestModel::rowCount(const QModelIndex &parent) const
{
  if(parent.isValid()) {
      return 0;
    }

  return m_data.size();
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()){
      return QVariant();
    }

  const ModelData& data = m_data[index.row()];

  switch (role) {
    case TestModel::Roles::FileNameRole:
      return data.fileName;
    case TestModel::Roles::LastModifiedRole:
      return data.LastModified;
    case TestModel::Roles::LastReadRole:
      return data.LastRead;
    case TestModel::Roles::FileUrlRole:
      return data.fileUrl;
    case TestModel::Roles::SizeRole:
      return data.size;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TestModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

  roles[FileNameRole] = "fileName";
  roles[LastModifiedRole] = "lastEdit";
  roles[FileUrlRole] = "fileUrl";
  roles[SizeRole] = "fileSize";
  roles[LastReadRole] = "lastRead";

  return roles;
}

void TestModel::insertInList(QFileInfo fileUrl)
{
  beginInsertRows(QModelIndex(), m_data.size(), m_data.size());

  ModelData data;

  data.LastModified = fileUrl.lastModified().toString("dd.MM.yyyy");
  data.LastRead     = fileUrl.lastRead().toString("dd.MM.yyyy");
  data.fileName     = fileUrl.fileName();
  data.size         = fileUrl.size();
  data.fileUrl      = fileUrl.filePath();

  m_data.push_back(data);

  endInsertRows();
  emit dataChanged();
}

void TestModel::add(const QString &fileUrl)
{
  auto x = [this](const QString &fileUrl) {
    QFileInfoList filePaths = imgInfo.getFilesPaths(fileUrl);

    for(int i = 0; i < filePaths.count(); i++) {
        changed(filePaths[i]);
      }
  };

  std::thread th(x,fileUrl);
  th.join();
}
