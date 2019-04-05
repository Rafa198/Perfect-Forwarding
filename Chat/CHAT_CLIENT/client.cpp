#include "client.h"
#include <thread>
#include <QUrl>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

chat_client::chat_client(boost::asio::io_service &io_service, tcp::resolver::iterator endpoint_iterator)
        : QObject (),
          io_service_(io_service),
        socket_(io_service)
{
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
                          boost::bind(&chat_client::handle_connect, this,
                                      boost::asio::placeholders::error, ++endpoint_iterator));
}

void chat_client::write(const chat_message &msg)
{
    io_service_.post(boost::bind(&chat_client::do_write, this, msg));
}

void chat_client::close()
{
    io_service_.post(boost::bind(&chat_client::do_close, this));
}

void chat_client::handle_connect(const boost::system::error_code &error, tcp::resolver::iterator endpoint_iterator)
{
    if (!error) {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                boost::bind(&chat_client::handle_read_header, this,
                                            boost::asio::placeholders::error));
    } else if (endpoint_iterator != tcp::resolver::iterator()) {
        socket_.close();
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
                              boost::bind(&chat_client::handle_connect, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
    }
}

void chat_client::handle_read_header(const boost::system::error_code &error)
{
    if (!error && read_msg_.decode_header()) {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                boost::bind(&chat_client::handle_read_body, this,
                                            boost::asio::placeholders::error));
    } else {
        do_close();
    }
}

void chat_client::handle_read_body(const boost::system::error_code &error)
{
    if (!error) {

        if (handle_) {
            handle_(read_msg_);
        }
        std::cout.write(read_msg_.body(), read_msg_.body_length());
        std::cout << "\n";
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                boost::bind(&chat_client::handle_read_header, this,
                                            boost::asio::placeholders::error));
    } else {
        do_close();
    }
}

void chat_client::do_write(chat_message msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 boost::bind(&chat_client::handle_write, this,
                                             boost::asio::placeholders::error));
    }
}

void chat_client::handle_write(const boost::system::error_code &error)
{
    if (!error) {
        write_msgs_.pop_front();
        if (!write_msgs_.empty()) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front().data(),
                                                         write_msgs_.front().length()),
                                     boost::bind(&chat_client::handle_write, this,
                                                 boost::asio::placeholders::error));
        }
    } else {
        do_close();
    }
}

void chat_client::do_close()
{
    socket_.close();
}

void chat_client::sendMessage(const QString& user, const QString& mes)
{
    chat_message message((user + "|").toStdString(), mes.toStdString());
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    message.encode_header();
    write(message);
}

void chat_client::sendFile(const QString &filename, const QString &username)
{
    auto x = [this](const QString &filePath, const QString &user) {
        unsigned long long sendBytes = 0;
        auto path = QUrl(filePath).toLocalFile();
        unsigned long long fileSize = boost::filesystem::file_size(path.toStdString());
        ifs_.open(path.toStdString(), std::ios::binary);

        if (!ifs_.is_open()) {
            throw std::runtime_error("Unable to open input file");
        }

        unsigned long long difference = 0;
        unsigned long long dSize = 0;

        while (sendBytes < fileSize) {

            difference = fileSize - sendBytes;
            dSize = file_buffer_size;


            if (difference < file_buffer_size) {
                dSize = difference;
            }

            ifs_.read(file_buffer, dSize);
            sendBytes += dSize;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            chat_message mes(user.toStdString() + "|", std::string(file_buffer, dSize), chat_message::Flags::FILE);
            write(mes);
        }
        ifs_.close();
    };

}
