#ifndef SERVER_H
#define SERVER_H

#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include <deque>
#include <set>
#include <iostream>
#include <fstream>

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

class chat_room
{
public:
  void join(chat_participant_ptr participant);
  void leave(chat_participant_ptr participant);
  void deliver(const ChatMessage& msg);

private:
  std::set<chat_participant_ptr> participants_;
  enum { max_recent_msgs = 1000 };
  chatMessageQueue recentMsgs_;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_session : public chat_participant, public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket))
    , room_(room)
  { }

  ~chat_session()
  { }

  void start();
  void deliver(const ChatMessage& msg);

private:
  void doReadHeader();
  void doReadBody();
  void doWrite();

private:
  tcp::socket socket_;
  chat_room& room_;
  ChatMessage readMsg_;
  chatMessageQueue writeMsgs_;
};

class chat_server
{
public:
  chat_server(boost::asio::io_service& io_context, const tcp::endpoint &endpoint)
    : acceptor_(io_context, endpoint)
  {
    do_accept();
  }

private:
  void do_accept();
  void StartAccept();
  void HandleAccept(chat_session* psession, boost::system::error_code const& errcode);

private:
  tcp::acceptor acceptor_;
  chat_room room_;
};

#endif // SERVER_H
