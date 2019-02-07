#include "server.h"

void ChatRoom::join(chatParticipant_ptr participant)
{
  std::cout << "SERVER| JOIN" << std::endl;
  participants_.insert(participant);
  for (auto msg: recentMsgs_)
    {
      participant->deliver(msg);
    }
}

void ChatRoom::leave(chatParticipant_ptr participant)
{
  std::cout << "SERVER| LEAVE" << std::endl;
  participants_.erase(participant);
}

void ChatRoom::deliver(const ChatMessage &msg)
{
  if(msg.getFlagToMessage() == ChatMessage::Flags::MESSAGE)
  recentMsgs_.push_back(msg);

  while (recentMsgs_.size() > max_recent_msgs)    
  {
    recentMsgs_.pop_front();
  }

  for (auto participant: participants_)   
  {
    participant->deliver(msg);
  }
}

void ChatSession::start()
{
  std::cout << "SERVER| START" << std::endl;
  room_.join(shared_from_this());
  doReadHeader();
}

void ChatSession::deliver(const ChatMessage &msg)
{
  bool writeInProgress = !writeMsgs_.empty();
  writeMsgs_.push_back(msg);

  if (!writeInProgress)
  {
    doWrite();
  }
}

void ChatSession::doReadHeader()
{
  auto self(shared_from_this());
  boost::asio::async_read(socket_, boost::asio::buffer(readMsg_.getHeader(), sizeof(ChatMessage::msgInfo)),
                          [this, self](boost::system::error_code ec, std::size_t /*length*/)
  {
      if (!ec)        
      {
        readMsg_.deleteBuf();
        readMsg_.allocate();
        doReadBody();
      }
      else          
      {
        std::cerr << "SERVER| ERROR: " << ec.message().c_str() << std::endl;
        room_.leave(shared_from_this());
      }
    });
}

void ChatSession::doReadBody()
{
  auto self(shared_from_this());
  boost::asio::async_read(socket_, boost::asio::buffer(readMsg_.getBody(), static_cast<unsigned int>(readMsg_.getBodySize())),
                          [this, self](boost::system::error_code ec, std::size_t /*length*/)
  {
      if (!ec)       
      {
        room_.deliver(readMsg_);
        doReadHeader();
      }
      else      
      {
        std::cerr << "SERVER| ERROR: " << ec.message().c_str() << std::endl;
        room_.leave(shared_from_this());
      }
    });
}

void ChatSession::doWrite()
{
  auto self(shared_from_this());
  boost::asio::async_write(socket_, boost::asio::buffer(writeMsgs_.front().getBuffer(), static_cast<unsigned int>(writeMsgs_.front().getSize())),
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
        std::cerr << "SERVER| ERROR: " << ec.message().c_str() << std::endl;
        room_.leave(shared_from_this());
      }
  });
}

void Server::do_accept()
{
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket)
      {
        if (!ec)
        {
          std::make_shared<ChatSession>(std::move(socket), room_)->start();
        }
        do_accept();
  });
}
