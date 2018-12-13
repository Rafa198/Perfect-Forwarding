#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QList>

#include <list>
#include <string>

class ChatModel : public QAbstractListModel
{
  Q_OBJECT
public:
  ChatModel(QObject *parent = nullptr);

  enum Roles {
    UserNameRole = Qt::UserRole + 1,
    MessageRole
  };

  virtual int rowCount(const QModelIndex &parent) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QHash<int, QByteArray> roleNames() const;

signals:
  void dataChanged();

public slots:


private:
    struct ModelData {
    std::string userName;
    std::string message;
  };

  QList<ModelData> modelData;
  //QString recipient;
};

#endif // CHATMODEL_H
