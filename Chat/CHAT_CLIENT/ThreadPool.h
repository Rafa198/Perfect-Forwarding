#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/exception/all.hpp>
#include <boost/exception_ptr.hpp>

class ThreadPool
{
public:

  typedef boost::function<bool ( const boost::exception_ptr&, const std::string&)> ExceptionHandler;

  ThreadPool(boost::asio::io_service& ioService, const std::size_t size
    , const ExceptionHandler exceptionHandler = ExceptionHandler())
    : work_(new boost::asio::io_service::work(ioService))
    , threadGroup_()
    , exceptionHandler_(exceptionHandler)
  {
    for (std::size_t i = 0; i != size; ++i)
    {
      addThread();
    }
  }

  ~ThreadPool()
  {
    stop();
  }

  void setExceptionHandler(const ExceptionHandler& exceptionHandler)
  {
    exceptionHandler_ = exceptionHandler;
  }

  void addThread()
  {
    threadGroup_.create_thread(boost::bind(&ThreadPool::run, this));
  }

  void stop()
  {
    if (work_)
    {
      delete work_;
      work_ = nullptr;
    }
    threadGroup_.join_all();
  }

private:

  void run()
  {
    bool stopped(false);
    while (!stopped)
    {
      try
      {
        work_->get_io_service().run();
        stopped = true;
      }
      catch(const std::exception& e)
      {
        if (exceptionHandler_)
        {
          stopped = exceptionHandler_(boost::current_exception(), e.what());
        }
      }
      catch (...)
      {
        if (exceptionHandler_)
        {
          stopped = exceptionHandler_(boost::current_exception(), "Unknown thread pool exception");
        }
      }
    }
  }

  boost::asio::io_service::work* work_;
  boost::thread_group threadGroup_; 

protected:
  ExceptionHandler exceptionHandler_;

}; 




#endif // THREAD_POOL_H
