#ifndef SERVER_H
#define SERVER_H

#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <deque>
#include <set>

#include <message.h>

using boost::asio::ip::tcp;

typedef std::deque<ChatMessage> chatMessageQueue;

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const ChatMessage& msg) = 0;
};
typedef std::shared_ptr<chat_participant> chat_participant_ptr;
//-------------------------------------------------
class chat_room
{
public:
  void join(chat_participant_ptr participant);
  void leave(chat_participant_ptr participant);
  void deliver(const ChatMessage& msg);

private:
  std::set<chat_participant_ptr> participants_;
  enum { max_recent_msgs = 100 };
  chatMessageQueue recentMsgs_;
};

//-------------------------------------
typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_session
  : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
      room_(room) {}

  void start();
  void deliver(const ChatMessage& msg);

private:
  void doReadHeader();
  void doReadBody();
  void doWrite();

  tcp::socket socket_;
  chat_room& room_;
  ChatMessage readMsg_;
  chatMessageQueue writeMsgs_;
};

class chat_server
{
public:
  chat_server(boost::asio::io_context& io_context,
      const tcp::endpoint &endpoint)
    : acceptor_(io_context, endpoint)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<chat_session>(std::move(socket), room_)->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
};


//class Server
//{
//public:
//  Server(boost::asio::io_service &io_service)
//    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234))
//  {
//    start_accept();
//  }

//  void handle_accept(con_handler::pointer connection, const boost::system::error_code &err);

//private:
//   tcp::acceptor acceptor_;
//   void start_accept();
//};

#endif // SERVER_H
