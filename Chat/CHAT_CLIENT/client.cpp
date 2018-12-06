#include "client.h"

#include <QDebug>

void Client::client_run(QString msg)
{
  tcp::socket socket(service);
  socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));
  boost::asio::write(socket, boost::asio::buffer(msg.toStdString()), error);

  if(!error)
  {
    qDebug() << "Client sent message!!" << endl;
  }
  else
  {
     qDebug() << "SEND FAILED!!!" << endl; // << error.message() << std::endl;
  }

 boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);

 if(error && error != boost::asio::error::eof)
 {
     qDebug() << "RECEIVE FAILED!!! \n"; // << error.message() << std::endl;
 }
 else
 {
     const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
     std::string str = data;
     messages.push_back(str);
     qDebug() << "RECEIVE DATA: " << data << endl;

 }
}
