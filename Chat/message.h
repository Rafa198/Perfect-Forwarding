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
  unsigned int userNameSize;
  unsigned int messageSize;
  char *data = nullptr;
  };
  ChatMessage() {}
  ChatMessage(std::string user, std::string msg)
  {
    info_.userName = user;
    info_.message = msg;
    msgInfo_.userNameSize = user.length();
    msgInfo_.messageSize = msg.length();
    msgInfo_.data = reinterpret_cast<char*>(&info_);
  }

  const char* getData() const noexcept
  {
    return msgInfo_.data;
  }
  char* getData()
  {
    return msgInfo_.data;
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
  }

  void setMessage(const std::string &message)
  {
    info_.message = message;
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
  msgInfo msgInfo_;
};

#endif // MESSAGE_H
