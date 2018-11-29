#ifndef CHATROOM_H
#define CHATROOM_H

#include <chatparticipant.h>
#include <chatmessage.h>

#include <set>
#include <deque>
#include <memory>

typedef std::shared_ptr<ChatParticipant> chat_participant_ptr;
typedef std::deque<ChatMessage> chatMessage_queue;

class ChatRoom
{
public:
  ChatRoom();
  void join(chat_participant_ptr participant);
  void leave(chat_participant_ptr participant);
  void deliver(const ChatMessage& msg);

private:
  std::set<chat_participant_ptr> participants_;
  enum { max_recent_msgs = 100 };
  chatMessage_queue recent_msgs_;
};

#endif // CHATROOM_H
