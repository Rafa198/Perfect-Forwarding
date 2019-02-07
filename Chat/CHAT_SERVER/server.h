#ifndef SERVER_H
#define SERVER_H

#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <deque>
#include <set>
#include <iostream>

#include <message.h>

using boost::asio::ip::tcp;

typedef std::deque<ChatMessage> chatMessageQueue;

class ChatParticipant
{
public:
  virtual ~ChatParticipant() {}
  virtual void deliver(const ChatMessage& msg) = 0;
};

typedef std::shared_ptr<ChatParticipant> chatParticipant_ptr;

class ChatRoom
{
public:
  void join(chatParticipant_ptr participant);
  void leave(chatParticipant_ptr participant);
  void deliver(const ChatMessage& msg);

private:
  enum { max_recent_msgs = 100};
  std::set<chatParticipant_ptr> participants_;  
  chatMessageQueue recentMsgs_;
};

typedef std::shared_ptr<ChatParticipant> chatParticipant_ptr;

class ChatSession : public ChatParticipant, public std::enable_shared_from_this<ChatSession>
{
public:
  ChatSession(tcp::socket socket, ChatRoom& room)
    : socket_(std::move(socket))
    , room_(room)
  { }

  ~ChatSession()
  { }

  void start();
  void deliver(const ChatMessage& msg);

private:
  void doReadHeader();
  void doReadBody();
  void doWrite();

private:
  tcp::socket socket_;
  ChatRoom& room_;
  ChatMessage readMsg_;
  chatMessageQueue writeMsgs_;
};

class Server
{
public:
  Server(boost::asio::io_service& service, const tcp::endpoint &endpoint)
    : acceptor_(service, endpoint)
  {
    do_accept();
  }

private:
  void do_accept();
  void startAccept();
  void handleAccept(ChatSession* psession, boost::system::error_code const& errcode);

private:
  tcp::acceptor acceptor_;
  ChatRoom room_;
};

#endif // SERVER_H
