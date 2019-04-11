#include "client.h"
#include <thread>
#include <QUrl>
#include <boost/filesystem.hpp>

chat_client::chat_client(boost::asio::io_service &service, tcp::resolver::iterator endpoint_iterator)
        : QObject()
          , m_io_service(service)
          , m_socket(service)
//          , fileBuffer()
{

    tcp::endpoint endpoint = *endpoint_iterator;
    m_socket.async_connect(endpoint,
                           boost::bind(&chat_client::handle_connect, this,
                                       boost::asio::placeholders::error, ++endpoint_iterator));
}

void chat_client::send_message(QString user, QString mes)
{
    chat_message message((user + "|").toStdString(), mes.toStdString());
    message.encode_header();
    write(message);
}

void chat_client::send_file(const QString &filePath1, const QString &user1)
{
    auto x = [this](const QString &filePath, const QString &user) {
        unsigned long long sendBytes = 0;
        auto path = QUrl(filePath).toLocalFile();
        unsigned long long fileSize = boost::filesystem::file_size(path.toStdString());
        m_file_stream.open(path.toStdString(), std::ios::binary);

        if (!m_file_stream.is_open()) {
            throw std::runtime_error("Unable to open input file");
        }

        unsigned long long difference = 0;
        unsigned long long dSize = 0;

        while (sendBytes < fileSize) {
            difference = fileSize - sendBytes;
            dSize = fileBufSize;

            if (difference < fileBufSize) {
                dSize = difference;
            }

            m_file_stream.read(fileBuffer, dSize);
            sendBytes += dSize;
            chat_message mes(user.toStdString() + "|", std::string(fileBuffer, dSize), chat_message::Flags::FILE);
            mes.encode_header();
            write(mes);
        }
        m_file_stream.close();
    };
    std::thread th(x, filePath1, user1);
    th.join();
}

void chat_client::write(const chat_message  &message)
{
    m_io_service.post(boost::bind(&chat_client::do_write, this, message));
}

void chat_client::close()
{
    m_io_service.post(boost::bind(&chat_client::close, this));
}

void chat_client::handle_connect(const boost::system::error_code &error, tcp::resolver::iterator endpoint_iterator)
{
    if (!error) {
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_read_message.data(), chat_message::header_length),
                                boost::bind(&chat_client::handle_read_header, this,
                                            boost::asio::placeholders::error));
    } else if (endpoint_iterator != tcp::resolver::iterator()) {
        m_socket.close();
        tcp::endpoint endpoint = *endpoint_iterator;
        m_socket.async_connect(endpoint,
                              boost::bind(&chat_client::handle_connect, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
    }
}

void chat_client::handle_read_header(const boost::system::error_code& error)
{
    if (!error && m_read_message.decode_header()) {
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_read_message.body(), m_read_message.body_length()),
                                boost::bind(&chat_client::handle_read_body, this,
                                            boost::asio::placeholders::error));
    } else {
        close();
    }
}

void chat_client::handle_read_body(const boost::system::error_code&error)
{
    if (!error) {

        if (m_read_handle) {
            m_read_handle(m_read_message);
        }
        std::cout.write(m_read_message.body(), m_read_message.body_length());
        std::cout << "\n";
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_read_message.data(), chat_message::header_length),
                                boost::bind(&chat_client::handle_read_header, this,
                                            boost::asio::placeholders::error));
    } else {
        close();
    }
}

void chat_client::do_write(chat_message msg)
{
    bool write_in_progress = !m_write_message_queue.empty();
    m_write_message_queue.push_back(msg);
    if (!write_in_progress) {
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(m_write_message_queue.front().data(),
                                                     m_write_message_queue.front().length()),
                                 boost::bind(&chat_client::handle_write, this,
                                             boost::asio::placeholders::error));
    }
}

void chat_client::handle_write(const boost::system::error_code &error)
{
    if (!error) {
        m_write_message_queue.pop_front();
        if (!m_write_message_queue.empty()) {
            boost::asio::async_write(m_socket,
                                     boost::asio::buffer(m_write_message_queue.front().data(),
                                                         m_write_message_queue.front().length()),
                                     boost::bind(&chat_client::handle_write, this,
                                                 boost::asio::placeholders::error));
        }
    } else {
        close();
    }
}

void chat_client::do_close()
{
    m_socket.close();

}
