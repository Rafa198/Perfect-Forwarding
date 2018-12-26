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
}

void chat_room::leave(chat_participant_ptr participant)
{
  std::cout << "SERVER: LEAVE" << std::endl;
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
  std::cout << "SERVER: START" << std::endl;
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
      boost::asio::buffer(readMsg_.getHeader(), sizeof(ChatMessage::msgInfo)),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
      std::cout << "SERVER|doReadHEADER| getHeader: " << readMsg_.getHeader() << " Size: " << sizeof(ChatMessage::msgInfo) << std::endl;

        if (!ec)
        {
          readMsg_.deleteBuf();
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
  auto self(shared_from_this());
  boost::asio::async_read(socket_,
      boost::asio::buffer(readMsg_.getBody(), readMsg_.getBodySize()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
      std::cout << "SERVER|doReadBODY| getbody: " << readMsg_.getBody() << " Size: " << readMsg_.getBodySize() << std::endl;
      if (!ec)
        {
            room_.deliver(readMsg_);
            doReadHeader();
        }
        else
        {
          std::cerr << "SERVER| ERROR: " << ec << std::endl;
          room_.leave(shared_from_this());
        }
    });
}

void chat_session::doWrite()
{
  auto self(shared_from_this());
  boost::asio::async_write(socket_,
      boost::asio::buffer(writeMsgs_.front().getBuffer(),
        writeMsgs_.front().getSize()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
      std::cout << "SERVER|doWrite| getbuffer: " << writeMsgs_.front().getBuffer() << " Size: " << writeMsgs_.front().getSize() << std::endl;

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
