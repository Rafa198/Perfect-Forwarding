#include "chatmodel.h"

#include <message.h>

ChatModel::ChatModel(QObject *parent) :
  QAbstractListModel(parent)
{
  //ChatMessage Cl1("Rafael", "Hello");
}

int ChatModel::rowCount(const QModelIndex &parent) const
{
  if(parent.isValid())
    {
      return 0;
    }

  return modelData.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid())
    {
      return QVariant();
    }

  const ModelData& data = modelData[index.row()];

  switch (role) {
    case ChatModel::Roles::UserNameRole:
      return QString::fromStdString(data.userName);
    case ChatModel::Roles::MessageRole:
      return QString::fromStdString(data.message);
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
