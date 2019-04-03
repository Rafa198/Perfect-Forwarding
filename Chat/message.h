#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <iostream>
#include <cstring>
class chat_message
{
public:


    enum { header_length = 4 };
    enum { max_body_length = 16777216 };// максимальный размер сообщения 16 Мб

    enum class Flags
    {
        MESSAGE,
        FILE
    };

    struct msgInfo
    {
        Flags flag = Flags::MESSAGE;
        unsigned int userNameSize = 0;
        unsigned int messageSize = 0;
    };

    chat_message(const std::string &username, const std::string &message, Flags flag = Flags::MESSAGE)
            : data_()
    {
        msgInfo_.userNameSize = username.length();
        msgInfo_.messageSize = message.length();
        msgInfo_.flag = flag;
        body_length_ = msgInfo_.userNameSize + msgInfo_.messageSize + sizeof(msgInfo);
        memcpy(data_, &msgInfo_, sizeof(msgInfo));
        memcpy(data_ + sizeof(msgInfo), username.c_str(), msgInfo_.userNameSize);
        memcpy(data_ + sizeof(msgInfo) + msgInfo_.userNameSize, message.c_str(), msgInfo_.messageSize);

    }
    chat_message()
            : data_()
              , body_length_(0)
    {

    }

    const char* data() const
    {
        return data_;
    }

    char* data()
    {
        return data_;
    }

    size_t length() const
    {
        return header_length + body_length_;
    }

    const char* body() const
    {
        return data_ + header_length;
    }

    char* body()
    {
        return data_ + header_length;
    }

    size_t body_length() const
    {
        return body_length_;
    }

    void body_length(size_t length)
    {
        body_length_ = length;
        if (body_length_ > max_body_length)
            body_length_ = max_body_length;
    }

    bool decode_header()
    {
        using namespace std; // For strncat and atoi.
        char header[header_length + 1] = "";
        strncat(header, data_, header_length);
        body_length_ = stoi(header);
        if (body_length_ > max_body_length) {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        using namespace std; // For sprintf and memcpy.
        char header[header_length + 1] = "";
        sprintf(header, "%4zu", body_length_);
        memcpy(data_, header, header_length);
    }

    Flags getFlagToMessage() const
    {
        return msgInfo_.flag;
    }
private:
    char data_[header_length + max_body_length];
    size_t body_length_;
    msgInfo msgInfo_;
};


#endif // MESSAGE_H
