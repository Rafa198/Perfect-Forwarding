#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>
#include <queue>
#include <fstream>

#include "message.hpp"


using namespace boost::asio;
using ip::tcp;
using ReadHandle = std::function<void(chat_message&)>;

typedef std::deque<chat_message> chatMessageQueue;

class chat_client : public QObject, public boost::enable_shared_from_this<chat_client>
{
  Q_OBJECT

public:
    chat_client(boost::asio::io_service &service, tcp::resolver::iterator endpoint_iterator);

    Q_INVOKABLE void send_message(QString user, QString mes);

    Q_INVOKABLE void send_file(const QString &filePath1, const QString &user1);


    void write(const chat_message &message);


    Q_INVOKABLE void close();

    inline void setReadHandle(const ReadHandle &handle)
    {
        m_read_handle = handle;
    }


private:
    void handle_connect(const boost::system::error_code &error, tcp::resolver::iterator endpoint_iterator);

    void handle_read_header(const boost::system::error_code &error);

    void handle_read_body(const boost::system::error_code &error);

    void do_write(chat_message msg);

    void handle_write(const boost::system::error_code &error);
    void do_close();
private:
    io_service &m_io_service;
    tcp::socket m_socket;
    chat_message m_read_message;

    chatMessageQueue m_write_message_queue;
    ReadHandle m_read_handle;
    std::ifstream m_file_stream;

private:
    static const unsigned int fileBufSize = 1024 * 16;
    char fileBuffer[fileBufSize];
};

#endif // CLIENT_H
