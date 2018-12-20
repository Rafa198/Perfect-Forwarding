#include "server.h"

#include <iostream>

void chat_room::join(chat_participant_ptr participant)
{
  std::cout << "SERVER: JOIN" << std::endl;
  participants_.insert(participant);
  for (auto msg: recentMsgs_)
    {
      participant->deliver(msg);
    }
    //std::cout << std::endl;
}

void chat_room::leave(chat_participant_ptr participant)
{
  std::cout << "SERVER: LEAVE" << std::endl;
  participants_.erase(participant);
}

void chat_room::deliver(const ChatMessage &msg)
{
      recentMsgs_.push_back(msg);
      //std::cout << "SERVER | " << msg.getUserName() << "TEXT: " << msg.getMessage() << std::endl;

  while (recentMsgs_.size() > max_recent_msgs)
    recentMsgs_.pop_front();

  for (auto participant: participants_)
    participant->deliver(msg);
}

void chat_session::start()
{
  std::cout << "SERVER: START" << std::endl;
  room_.join(shared_from_this());
  doReadHeader();
}

void chat_session::deliver(const ChatMessage &msg)
{
  //std::cout << "SERVER: DELIVER Writing..." << std::endl;
  bool writeInProgress = !writeMsgs_.empty();
  writeMsgs_.push_back(msg);

  if (!writeInProgress)
  {
    doWrite();
  }
}

void chat_session::doReadHeader()
{
  //std::cout << "SERVER: start doReadHeader..." << std::endl;
  auto self(shared_from_this());
  boost::asio::async_read(socket_,
      boost::asio::buffer(readMsg_.getMessageInfo(), sizeof(ChatMessage::msgInfo)),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec /*&& read_msg_.decode_header()*/)
        {
          readMsg_.allocate();
          doReadBody();
        }
        else
        {
          std::cerr << "SERVER| ERROR: " << ec << std::endl;
          room_.leave(shared_from_this());
        }
    });
}

void chat_session::doReadBody()
{
  //std::cout << "SERVER| start doReadBody..." << std::endl;
  auto self(shared_from_this());
  boost::asio::async_read(socket_,
      boost::asio::buffer(readMsg_.getData(), readMsg_.getAllSize()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
            readMsg_.parseBuffer();
          std::cout << "SERVER | " << readMsg_.getUserName() << " TEXT: " << readMsg_.getMessage() << std::endl;
          room_.deliver(readMsg_);
          doReadHeader();
        }
        else
        {
          std::cerr << "SERVER| ERROR: " << ec << std::endl;
          room_.leave(shared_from_this());
        }
        readMsg_.deallocate();
    });
}

void chat_session::doWrite()
{
  //std::cout << "SERVER: start doWRITE..." << std::endl;
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
          std::cerr << "SERVER| ERROR: " << ec << std::endl;
          room_.leave(shared_from_this());
        }
      });
}
