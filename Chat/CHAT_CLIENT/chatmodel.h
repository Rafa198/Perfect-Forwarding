#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QList>

class ChatModel : public QAbstractListModel
{
  Q_OBJECT

public:
  ChatModel(QObject *parent);

  enum Roles {
    UserNameRole = Qt::UserRole + 1,
    MessageRole
  };

  virtual int rowCount(const QModelIndex &parent) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QHash<int, QByteArray> roleNames() const;

  void add(const QString &text);

signals:
  void dataChanged();
  void changed(QString userName, QString message);

public slots:
  void insertInList(const QString &userName,const QString &message);

private:
    struct ModelData {
    QString userName;
    QString message;
  };

  QList<ModelData> modelData_;
};

#endif // CHATMODEL_H
