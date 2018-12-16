#include "server.h"

//void Server::handle_accept(con_handler::pointer connection, const boost::system::error_code &err)
//{
//  if (!err)
//    {
//      connection->start();
//    }
//  start_accept();
//}

//void Server::start_accept()
//{
//  con_handler::pointer connection = con_handler::create(acceptor_.get_io_service());

//  acceptor_.async_accept(connection->socket(),
//                         boost::bind(&Server::handle_accept,
//                         this,
//                         connection,
//                         boost::asio::placeholders::error));
//}

void chat_room::join(chat_participant_ptr participant)
{
  participants_.insert(participant);
  for (auto msg: recentMsgs_)
    participant->deliver(msg);
}

void chat_room::leave(chat_participant_ptr participant)
{
  participants_.erase(participant);
}

void chat_room::deliver(const ChatMessage &msg)
{
  recentMsgs_.push_back(msg);

  while (recentMsgs_.size() > max_recent_msgs)
    recentMsgs_.pop_front();

  for (auto participant: participants_)
    participant->deliver(msg);
}

void chat_session::start()
{
  room_.join(shared_from_this());
  doReadHeader();
}

void chat_session::deliver(const ChatMessage &msg)
{
  bool writeInProgress = !writeMsgs_.empty();
  writeMsgs_.push_back(msg);

  if (!writeInProgress)
  {
    doWrite();
  }
}

void chat_session::doReadHeader()
{
  auto self(shared_from_this());
  boost::asio::async_read(socket_,
      boost::asio::buffer(readMsg_.getData(), ChatMessage::header_length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec /*&& read_msg_.decode_header()*/)
        {
          doReadBody();
        }
        else
        {
          room_.leave(shared_from_this());
        }
    });
}

void chat_session::doReadBody()
{
  auto self(shared_from_this());
  boost::asio::async_read(socket_,
      boost::asio::buffer(readMsg_.getData() + ChatMessage::header_length, readMsg_.max_body_length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          room_.deliver(readMsg_);
          doReadHeader();
        }
        else
        {
          room_.leave(shared_from_this());
        }
    });
}

void chat_session::doWrite()
{
  auto self(shared_from_this());
  boost::asio::async_write(socket_,
      boost::asio::buffer(writeMsgs_.front().getData(),
        writeMsgs_.front().getAllSize()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          writeMsgs_.pop_front();

          if (!writeMsgs_.empty())
          {
            doWrite();
          }
        }
        else
        {
          room_.leave(shared_from_this());
        }
      });
}
