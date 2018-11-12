#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <functional>
#include <random>
#include <future>


#include <QString>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>
#include <QVector>
#include <QDebug>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "useqtthread.h"

using std::cout;
using std::endl;

std::default_random_engine rng;
std::mutex mtx;
std::mutex mtx1;
std::vector<std::string> vec;
boost::asio::io_service ioserv;

int Srandom(int a, int b);
void pushInVector();
void Print(char a);
std::string func(std::future<int> &f);

const int stringLength = 10;
const int numElementsForAddOneThread = 1000;

int main()
{
    boost::thread t{pushInVector};
    std::thread t1(pushInVector);
    QFuture<void> future = QtConcurrent::run(pushInVector);
    //useQtThread qtThread(pushInVector);
    //pushInVector();

    // io_service strand //
    boost::asio::io_context::strand strand_1(ioserv);
    ioserv.post(strand_1.wrap(boost::bind(pushInVector)));
    ioserv.run();

    t.join();
    t1.join();
    future.waitForFinished();
    //qtThread.start();

    return 0;
}

std::string func(std::future<int> &f)
{
  int i = f.get();
  std::string read = vec[i];

  cout << "read 'i' element of vector"
       << " i:" << i << " value = " << read << endl
       << "Executing in THREAD: " << QThread::currentThreadId()
       << "(" << std::this_thread::get_id() << ")" << endl
       << "return element in thread" << endl;
  return read;
}

int Srandom(int a, int b)
{
  std::uniform_int_distribution<int> dist_a_b(a, b);
  return dist_a_b(rng);
}

void pushInVector()
{
  mtx.lock();
  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::string str;

  cout << "\tID THREAD: " << QThread::currentThreadId()
       << " (" << std::this_thread::get_id() << ")" << endl;

    for(int i = 0; i < numElementsForAddOneThread; i++)
    {
        for(int j = 0; j < stringLength; j++)
        {
           char ch = Srandom(65,90);
           str += ch;
        }
        cout << str << "\tID THREAD: " << QThread::currentThreadId()
             << " (" << std::this_thread::get_id() << ")" << endl;

        vec.push_back(str);
        str = "";
    }

    std::promise<int> prms;
    std::future <int> f = prms.get_future();
    std::future<std::string> ftr = std::async(std::launch::async, func, std::ref(f));
    prms.set_value(Srandom(0, vec.size() - 1));
    std::string s = ftr.get();

    cout << "returned value " << s <<  " in THREAD: " << QThread::currentThreadId()
         << " (" << std::this_thread::get_id() << ")" << endl;

    cout << "------------------------------------" << endl;
    cout << "SIZE OF VECTOR " << vec.size()        << endl;
    cout << "------------------------------------" << endl;

    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
