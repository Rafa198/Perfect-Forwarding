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

  int rowCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

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

  QList<ModelData> m_model_data;
};

#endif // CHATMODEL_H
