#ifndef CHATPARTICIPANT_H
#define CHATPARTICIPANT_H

#include <chatmessage.h>

class ChatParticipant
{
public:
  ChatParticipant();
  virtual ~ChatParticipant() {}
  virtual void deliver(const ChatMessage& msg) = 0;
};

#endif // CHATPARTICIPANT_H
