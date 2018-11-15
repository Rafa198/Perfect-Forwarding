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
std::vector<std::string> vec;
std::mutex mtx;
std::mutex mtx1;
boost::asio::io_service ioserv;

void readAndReturn();
int Srandom(int a, int b);
void pushInVector();
std::string func(std::future<int> &f);

const int stringLength = 10;
const int numElements = 100;

int main()
{
    boost::thread t1{pushInVector}, t2{readAndReturn};

    std::thread st1(pushInVector), st2(readAndReturn);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::thread st3(readAndReturn);
    //QFuture<void> future = QtConcurrent::run(pushInVector);
    //QFuture<void> future1 = QtConcurrent::run(readAndReturn);
    //useQtThread qtThread(pushInVector);
    //pushInVector();

    // io_service strand //
    //boost::asio::io_context::strand strand_1(ioserv);
    //ioserv.post(strand_1.wrap(boost::bind(pushInVector)));
    //ioserv.run();
    //

    t1.join();
    t2.join();
    st1.join();
    st2.join();
    st3.join();
    //future.waitForFinished();
    //future1.waitForFinished();
    //qtThread.start();

    return 0;
}

int Srandom(int a, int b)
{
  std::uniform_int_distribution<int> dist_a_b(a, b);
  return dist_a_b(rng);
}

std::string func(std::future<int> &f)
{

  int i = f.get();
  mtx.lock();
  std::string read = vec[i];
  mtx.unlock();

  cout << "read 'i' element of vector"
       << " i:" << i << " value = " << read << endl
       << "Executing in THREAD: " << QThread::currentThreadId()
       << "(" << std::this_thread::get_id() << ")" << endl
       << "return element in thread" << endl;
  return read;

}
void readAndReturn()
{
  std::promise<std::string> prms1;
  auto fut = prms1.get_future();

  cout << " read THREAD: " << QThread::currentThreadId()
       << " (" << std::this_thread::get_id() << ")" << endl;

  cout << "size = " << vec.size() << endl;
  //mtx.lock();

  int index = Srandom(0, vec.size() - 1);
  //mtx.unlock();
  cout << "index = " << index << endl;

  auto fun = [index, &prms1]()
  {
      cout << " read fun THREAD: " << QThread::currentThreadId()
           << " (" << std::this_thread::get_id() << ")" << endl;


      if(index>=0 && index < vec.size())
        {
          mtx.lock();
          std::string read = vec[index];
          mtx.unlock();

          prms1.set_value(read);
          return;
        }

      prms1.set_value("");
  };

  cout << "wait";

  //std::async(std::launch::async, fun);
  std::thread th2(fun);
  th2.join();

  auto s = fut.get();
  cout << "\ngot!\n";

//  std::promise<int> prms;
//  std::future <int> f = prms.get_future();
//  std::future<std::string> ftr = std::async(std::launch::async, func, std::ref(f));
//  prms.set_value(Srandom(0, vec.size() - 1));
//  std::string s = ftr.get();

  cout << "returned value " << s <<  " in THREAD: " << QThread::currentThreadId()
       << " (" << std::this_thread::get_id() << ")" << endl;

}

void pushInVector()
{
  std::string str;

  cout << "\tID THREAD: " << QThread::currentThreadId()
       << " (" << std::this_thread::get_id() << ")" << endl;

    for(int i = 0; i < numElements; i++)
    {
        for(int j = 0; j < stringLength; j++)
        {
           char ch = Srandom(65,90);
           str += ch;
        }

        cout << str << "\tID THREAD: " << QThread::currentThreadId()
             << " (" << std::this_thread::get_id() << ")" << endl;

        mtx.lock();
        vec.push_back(str);
        mtx.unlock();

        str = "";
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
