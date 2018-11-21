#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>


class TestModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum Roles {
    DisplayRole = Qt::UserRole + 1,
    DecorationRole
  };

  TestModel(QObject *parent = nullptr);

  virtual int rowCount(const QModelIndex &parent) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QHash<int, QByteArray> roleNames() const;

  Q_INVOKABLE void add();

private:

  struct ModelData{
    QString fileName;
    QString fileUrl;
    QString LastModified;
    QString LastRead;
    int     size;
  };

  QList <ModelData> m_data;
};

#endif // TESTMODEL_H
