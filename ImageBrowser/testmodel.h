#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "imageinfo.h"

class TestModel : public QAbstractListModel
{
  Q_OBJECT

public:

  enum Roles {
    FileNameRole = Qt::UserRole + 1,
    FileUrlRole,
    LastModifiedRole,
    LastReadRole,
    SizeRole
  };

  TestModel(QObject *parent = nullptr);

  virtual int rowCount(const QModelIndex &parent) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QHash<int, QByteArray> roleNames() const;
  Q_INVOKABLE void add(const QString &fileUrl);

signals:
    void dataChanged();
    void changed(QFileInfo);

public slots:
    void insertInList(QFileInfo fileUrl);

private:

    struct ModelData {
    QString fileName;
    QString fileUrl;
    QString LastModified;
    QString LastRead;
    qint64  size;
  };

  QList<ModelData> m_data;
  QString folderUrl_;
  ImageInfo imgInfo;
};

#endif // TESTMODEL_H
