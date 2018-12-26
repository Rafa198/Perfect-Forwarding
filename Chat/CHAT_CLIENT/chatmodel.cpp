#include "chatmodel.h"

#include <QListIterator>

#include <thread>

ChatModel::ChatModel(QObject *parent)
  : QAbstractListModel(parent)
{
  QObject::connect(this, SIGNAL(changed(QString,QString)), this, SLOT(insertInList(QString,QString)),Qt::QueuedConnection);
}

int ChatModel::rowCount(const QModelIndex &parent) const
{
  return modelData_.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid())
    {
      return QVariant();
    }

  const ModelData& data = modelData_[index.row()];
  switch (role) {
    case ChatModel::Roles::UserNameRole:
      return data.userName;
    case ChatModel::Roles::MessageRole:
      return data.message;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

  roles[UserNameRole] = "userName";
  roles[MessageRole]  = "message";

  return roles;
}

void ChatModel::add(const QString &text)
{
   auto x = [this](const QString &str)
   {
   QStringList str1 = str.split('|');
   changed(str1[0], str1[1]);
   };
    std::thread th(x,text);
    th.join();
}

void ChatModel::insertInList(const QString &userName,const QString &message)
{
  beginInsertRows(QModelIndex(), modelData_.size(), modelData_.size());

  ModelData data_;
  data_.userName = userName;
  data_.message  = message;
  modelData_.push_back(data_);

  endInsertRows();
  emit dataChanged();
}
