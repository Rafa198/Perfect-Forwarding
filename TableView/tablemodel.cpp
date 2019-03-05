#include "tablemodel.h"

#include <QStringList>
#include <QFile>
#include <QUrl>

TableModel::TableModel(QObject *parent)
{
  QObject::connect(this, SIGNAL(changed(QString,QString,QString)),
                   this, SLOT(insertInList(QString,QString,QString)), Qt::QueuedConnection);
}

void TableModel::add(const QString &filePath)
{
  QString line;
  auto path = QUrl(filePath).toLocalFile();

  QFile file(path);
  file.open(QIODevice::ReadOnly);

  if(!file.isOpen())
  {
    throw std::runtime_error("Unable to open file");
  }

  while(!file.atEnd())
  {
    line = file.readLine();
    QStringList lines = line.split(';');
    changed(lines[0], lines[1], lines[2]);
    // index value
    // 0     userID
    // 1     userName
    // 2     userPhoneNumber
  }
  file.close();
}

void TableModel::insertInList(const QString &userID, const QString &userName, const QString &userPhoneNumber)
{
  beginInsertRows(QModelIndex(), modelData_.size(), modelData_.size());

  ModelData data_;
  data_.userId = userID;
  data_.userName  = userName;
  data_.UserPhoneNumber = userPhoneNumber;

  modelData_.push_back(data_);

  endInsertRows();
  emit dataChanged();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
  return modelData_.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
  return 3;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid())
  {
    return QVariant();
  }

  switch (role) {
    case TableModel::Roles::UserIdRole: return modelData_[index.row()].userId;
    case TableModel::Roles::UserNameRole: return modelData_[index.row()].userName;
    case TableModel::Roles::UserPhoneNumberRole: return modelData_[index.row()].UserPhoneNumber;
    default: return QVariant();
  }
}

QHash<int, QByteArray> TableModel::roleNames() const
{
  QHash<int, QByteArray> roles;

  roles[UserIdRole] = "userID";
  roles[UserNameRole]  = "userName";
  roles[UserPhoneNumberRole] = "userPhoneNumber";

  return roles;
}
