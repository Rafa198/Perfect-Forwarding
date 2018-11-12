#ifndef USEQTTHREAD_H
#define USEQTTHREAD_H

#include <QObject>
#include <QThread>

class useQtThread : public QThread
{
public:

  useQtThread(void f());
public slots:

signals:

protected:
  //void run()  override;
};

#endif // USEQTTHREAD_H
