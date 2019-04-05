#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <cstdlib>
#include <deque>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <queue>
#include <fstream>
#include "../message.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

using ip::tcp;
using ReadHandle = std::function<void(chat_message&)>;

typedef std::deque<chat_message> chat_message_queue;

class chat_client : public QObject, public boost::enable_shared_from_this<chat_client>
{
Q_OBJECT
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
    ReadHandle handle_;
    std::ifstream ifs_;
    static const unsigned int file_buffer_size = 1024 * 16;
    char file_buffer[file_buffer_size];

public:

    chat_client(boost::asio::io_service& io_service,
            tcp::resolver::iterator endpoint_iterator);

    void write(const chat_message& msg);

    Q_INVOKABLE void close();

    Q_INVOKABLE void sendMessage(const QString& user, const QString& mes);

    Q_INVOKABLE void sendFile(const QString &filename, const QString &username);

    Q_INVOKABLE void do_close();

    inline void setReadHandle(const ReadHandle& handle) { handle_ = handle; }

private:

    void handle_connect(const boost::system::error_code& error,
            tcp::resolver::iterator endpoint_iterator);

    void handle_read_header(const boost::system::error_code& error);

    void handle_read_body(const boost::system::error_code& error);

    void do_write(chat_message msg);

    void handle_write(const boost::system::error_code& error);
};


#endif // CLIENT_H
