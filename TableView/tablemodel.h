#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QString>

class TableModel : public QAbstractListModel
{
  Q_OBJECT

public:
  TableModel(QObject *parent = nullptr);
  Q_INVOKABLE void add(const QString &text);

  enum Roles {
    UserIdRole = Qt::UserRole,
    UserNameRole,
    UserPhoneNumberRole
  };

signals:
  void dataChanged();
  void changed(QString userID, QString userName, QString userPhoneNumber);

public slots:
  void insertInList(const QString &userID,const QString &userName, const QString &userPhoneNumber);

private:
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  virtual QHash<int, QByteArray> roleNames() const;

private:
  struct ModelData {
    QString userId;
    QString userName;
    QString UserPhoneNumber;
  };

  QList<ModelData> modelData_;
};

#endif // TABLEMODEL_H
