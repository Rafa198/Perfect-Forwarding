#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>

class ChatMessage {
public:

  enum class Flags {
    MESSAGE,
    FILE
  };

  struct msgInfo {
    Flags flag = Flags::MESSAGE;
    unsigned int userNameSize = 0;
    unsigned int messageSize = 0;
  };

  ChatMessage()
    : buffer_(nullptr)
  { }

  ChatMessage(ChatMessage&&) = delete;

  ChatMessage(const ChatMessage &chatMessage)
    : buffer_(nullptr)
    , msgInfo_(chatMessage.msgInfo_)
  {
    unsigned int size = msgInfo_.userNameSize + msgInfo_.messageSize + sizeof (msgInfo);
    buffer_ = new char[size];
    memcpy(buffer_, chatMessage.buffer_, size);
  }

 ChatMessage(const std::string &userName, const std::string &message, Flags flag = Flags::MESSAGE)
   : buffer_(nullptr)
 {
   msgInfo_.userNameSize = userName.length();
   msgInfo_.messageSize = message.length();
   msgInfo_.flag = flag;

   buffer_ = new char[msgInfo_.userNameSize + msgInfo_.messageSize + sizeof (msgInfo)];

   memcpy(buffer_, &msgInfo_, sizeof (msgInfo));
   memcpy(buffer_ + sizeof (msgInfo), userName.c_str(), msgInfo_.userNameSize);
   memcpy(buffer_ + sizeof (msgInfo) + msgInfo_.userNameSize, message.c_str(), msgInfo_.messageSize);
 }

  ~ChatMessage()
  {
    if(buffer_)     
    {
      delete[] buffer_;
      buffer_ = nullptr;
    }
  }

  void allocate()
  {
    if(buffer_ == nullptr)     
    {
      unsigned long long int size = msgInfo_.messageSize + msgInfo_.userNameSize + sizeof(msgInfo);

      try
      {
        buffer_ = new char[size];
        memcpy(buffer_, &msgInfo_, sizeof (msgInfo));
      }
      catch (std::exception ex)
      {
        throw std::runtime_error(ex.what());
      }
    }
  }

  void deleteBuf()
  {
    if(buffer_)     
    {
      delete[] buffer_;
      buffer_ = nullptr;
    }
  }

  char* getBuffer()
  {
    return buffer_;
  }

  char* getHeader()
  {
    return reinterpret_cast<char*>(&msgInfo_);
  }

  char* getBody()
  {
    return (buffer_ + sizeof (msgInfo));
  }

  unsigned int getBodySize() const noexcept
  {
    return (msgInfo_.messageSize + msgInfo_.userNameSize);
  }

  unsigned int getSize() const
  {
    return (getBodySize() + sizeof (msgInfo));
  }

  Flags getFlagToMessage() const
  {
    return msgInfo_.flag;
  }

private:
  char* buffer_;
  msgInfo msgInfo_;
};

#endif // MESSAGE_H
