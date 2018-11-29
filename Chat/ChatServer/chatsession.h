#ifndef CHATSESSION_H
#define CHATSESSION_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <chatparticipant.h>
#include <chatroom.h>
#include <chatmessage.h>


typedef std::deque<ChatMessage> chatMessage_queue;
using boost::asio::ip::tcp;

class ChatSession : public ChatParticipant,
    public std::enable_shared_from_this<ChatSession>
{
public:
  ChatSession(tcp::socket socket, ChatRoom& room)
    : socket_(std::move(socket)),
      room_(room)
  {
  }

  void start();
  void deliver(const ChatMessage& msg);

private:
  void do_read_header();
  void do_read_body();
  void do_write();

  tcp::socket socket_;
  ChatRoom& room_;
  ChatMessage read_msg_;
  chatMessage_queue write_msgs_;
};

#endif // CHATSESSION_H
