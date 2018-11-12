#ifndef USEQTTHREAD_H
#define USEQTTHREAD_H

#include <QObject>
#include <QThread>

class useQtThread : public QThread
{
public:
  useQtThread(void f());

};

#endif // USEQTTHREAD_H
