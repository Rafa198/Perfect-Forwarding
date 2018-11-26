#include <QtDebug>

#include <iostream>
#include <vector>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
io_service service;

class TalkToClient;

typedef boost::shared_ptr<TalkToClient> client_ptr;
typedef std::vector<client_ptr> array;
array clients;

void update_clients();

class TalkToClient : public boost::enable_shared_from_this<TalkToClient>
    , boost::noncopyable {

  typedef TalkToClient self_type;
  TalkToClient() : sock_(service), started_(false), timer_(service), clients_changed_(false){

}
public:
  typedef boost::system::error_code error_code;
  typedef boost::shared_ptr<TalkToClient> ptr;

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string username_;
    deadline_timer timer_;
    boost::posix_time::ptime last_ping;
    bool clients_changed_;
};
