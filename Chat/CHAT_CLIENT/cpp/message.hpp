
#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;


#include <cstdio>
#include <cstdlib>
#include <cstring>

class chat_message
{
public:
    enum { header_length = 4 };
    enum { max_body_length = 1024 };// максимальный размер сообщения 16 Мб
    //

    enum class Flags
    {
        MESSAGE,
        FILE
    };

    struct MessageInfo
    {
        Flags flag = Flags::MESSAGE;
        unsigned int userNameSize = 0;
        unsigned int messageSize = 0;
    };
private:
    char *m_data;
    size_t m_body_length;
    MessageInfo m_message_info;
public:
    chat_message(const std::string &username, const std::string &message, Flags flag = Flags::MESSAGE)
            : m_data(nullptr)
    {
        m_message_info.userNameSize = username.length();
        m_message_info.messageSize = message.length();
        m_message_info.flag = flag;
        m_body_length = m_message_info.userNameSize + m_message_info.messageSize + sizeof(MessageInfo);
        m_data = new char[m_message_info.userNameSize + m_message_info.messageSize + sizeof (MessageInfo)];
        memcpy(m_data, &m_message_info, sizeof(MessageInfo));
        memcpy(m_data + sizeof(MessageInfo), username.c_str(), m_message_info.userNameSize);
        memcpy(m_data + sizeof(MessageInfo) + m_message_info.userNameSize, message.c_str(), m_message_info.messageSize);
    }
    chat_message()
              : m_body_length(0)
    {

    }

    const char* data() const
    {
        return m_data;
    }

    char* data()
    {
        return m_data;
    }

    size_t length() const
    {
        return header_length + m_body_length;
    }

    const char* body() const
    {
        return m_data + header_length;
    }

    char* body()
    {
        return m_data + header_length;
    }

    size_t body_length() const
    {
        return (m_message_info.messageSize + m_message_info.userNameSize);
    }

    void body_length(size_t length)
    {
        m_body_length = length;
        if (m_body_length > max_body_length)
            m_body_length = max_body_length;
    }

    bool decode_header()
    {
        using namespace std; // For strncat and atoi.
        char header[header_length + 1] = "";
        strncat(header, m_data, header_length);
        m_body_length = std::stoi(header);
        if (m_body_length > max_body_length)
        {
            m_body_length = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        using namespace std; // For sprintf and memcpy.
        char header[header_length + 1] = "";
        sprintf(header, "%4zu", m_body_length);
        memcpy(m_data, header, header_length);
    }
    Flags get_flag2message() const
    {
        return m_message_info.flag;
    }

};

typedef std::deque<chat_message> chat_message_queue;

#endif //MESSAGE_HPP
