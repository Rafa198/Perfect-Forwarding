#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class ChatMessage {
public:
  enum { header_length = 4 };
  enum { max_body_length = 512 };

  struct info {
    std::string userName;
    std::string message;
  };

  struct msgInfo {
  unsigned int userNameSize = 0;
  unsigned int messageSize = 0;
  //char *data = nullptr;
  };

  ChatMessage()
    : body_length_(0)
    , buffer_(nullptr)
  {}

  ChatMessage(const ChatMessage &chatMessage)
    : body_length_(chatMessage.body_length_)
    , buffer_(nullptr)
    , info_(chatMessage.info_)
    , msgInfo_(chatMessage.msgInfo_)
  {
    if(chatMessage.buffer_)
      {
        allocate();
        fillBuffer();
      }
  }

  ChatMessage(ChatMessage &&chatMessage)
    : body_length_(chatMessage.body_length_)
    , buffer_(std::move(chatMessage.buffer_))
    , info_(std::move(chatMessage.info_))
    , msgInfo_(std::move(chatMessage.msgInfo_))
  {}

  ~ChatMessage()
  {
    deallocate();
  }

  void allocate()
  {
    if(buffer_ == nullptr)
    buffer_ = new char[getAllSize()];
  }
  void parseBuffer()
  {
    if(buffer_)
      {
        info_.userName = std::string(buffer_,msgInfo_.userNameSize);
        info_.message = std::string(buffer_ + msgInfo_.userNameSize, msgInfo_.messageSize);
      }
  }
  void fillBuffer()
  {
    if(buffer_)
      {
        memcpy(buffer_,info_.userName.c_str(), msgInfo_.userNameSize);
        memcpy(buffer_ + msgInfo_.userNameSize, info_.message.c_str(), msgInfo_.messageSize);

      }
  }
  void deallocate()
  {
    delete[] buffer_;
    buffer_ = nullptr;
  }
  char * getMessageInfo()
  {
    return reinterpret_cast<char*>(&msgInfo_);
  }
  std::size_t body_length() const
  {
      return body_length_;
  }

  void body_length(std::size_t new_length)
    {
      body_length_ = new_length;
      if (body_length_ > max_body_length)
        body_length_ = max_body_length;
    }
  const char* getData() const noexcept
  {
    return buffer_;
  }

  char* getData()
  {
    return buffer_;
  }

  const std::string &getUserName() const noexcept
  {
    return info_.userName;
  }

  const std::string &getMessage() const noexcept
  {
    return info_.message;
  }

  void setUserName(const std::string &userName)
  {
    info_.userName = userName;
    msgInfo_.userNameSize = userName.length();
  }

  void setMessage(const std::string &message)
  {
    info_.message = message;
    msgInfo_.messageSize = message.length();
  }

  const info &getStruct() const noexcept
  {
    return info_;
  }

  unsigned int getUserNameSize() const noexcept
  {
    return msgInfo_.userNameSize;
  }

  unsigned int getMessageSize() const noexcept
  {
    return msgInfo_.messageSize;
  }

  unsigned int getAllSize() const noexcept
  {
    return getMessageSize() + getUserNameSize();
  }

private:
  info info_;
  char * buffer_;
  msgInfo msgInfo_;
  std::size_t body_length_;
};

#endif // MESSAGE_H
