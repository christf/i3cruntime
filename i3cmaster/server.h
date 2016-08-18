#ifndef SERVER_CLASS
#define SERVER_CLASS

#include <string>
#include <iostream>
#include <string>
#include <cstring> 	// used for memset.
#include <arpa/inet.h> 	// for inet_ntop function
#include <netdb.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>
#include <list>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <thread>
#include <mutex>
#include "connection.h"
#include <vector>
#include <boost/asio.hpp>
#include <libconfig.h++>

#include <string>
#include <iostream>
#include <cstring> 	// used for memset.
#include <arpa/inet.h> 	// for inet_ntop function
#include <netdb.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>
#include <list>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <thread>
#include <mutex>
#include "sys/i2c/i2cpacket.h"
#include "sys/i2c/i2caddress.h"
#include <libconfig.h++>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <queue>


#include <cstdint>
#include "./sys/i2c/i2cpacket.h"

using boost::asio::ip::tcp;
using namespace libconfig;

class Server
{
public:
  Server(boost::asio::io_service& io_service, short port,  std::shared_ptr<std::deque<std::shared_ptr<i3c::sys::i2c::I2CPacket>>> queuepointer );
  
private:
  void do_accept();
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  std::shared_ptr<std::deque<std::shared_ptr<i3c::sys::i2c::I2CPacket>>> m_queue;
};



class Session
  : public std::enable_shared_from_this<Session>
{
public:
  Session(tcp::socket socket,std::shared_ptr<std::deque<std::shared_ptr<i3c::sys::i2c::I2CPacket>>> queuepointer);
  void start();

private:
  bool parsesuccess_;
  void parse() ;
  void do_read();
  void do_write(bool success);

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  std::shared_ptr<std::deque<std::shared_ptr<i3c::sys::i2c::I2CPacket>>> m_queue;
};



#endif
