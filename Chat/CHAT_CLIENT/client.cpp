#include "client.h"

#include <QDebug>

void Client::client_run(QString msg)
{
  //qDebug() << "TEXT---------A " << msg << endl;

  tcp::socket socket(service);

  socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234 ));
// request/message from client
  std::string txt = msg.toStdString();

  boost::asio::write(socket, boost::asio::buffer(txt), error);
  if(!error)
  {
    qDebug() << "Client sent message!!" << endl;
  }
  else
  {
     qDebug() << "SEND FAILED!!!" << endl; // << error.message() << std::endl;
  }
// getting response from server
 boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
 if(error && error != boost::asio::error::eof)
 {
     qDebug() << "RECEIVE FAILED!!! \n"; // << error.message() << std::endl;
 }
 else
 {
     const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
     qDebug() << " RECEIVE DATA: " << data << endl;
 }
}
