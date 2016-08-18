#include "server.h"



  Server::Server(boost::asio::io_service& io_service, short port,  std::deque<std::shared_ptr<i3c::sys::i2c::I2CPacket>> packetqueue)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), m_queue(packetqueue), 
      socket_(io_service)
  {
    do_accept();
  }


  void Server::do_accept()
  {
    acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec)
        {
          if (!ec)
          {
            std::make_shared<Session>(std::move(socket_),m_queue)->start();
          }

          do_accept();
        });
  }

  


  Session::Session(tcp::socket socket,std::deque<std::shared_ptr<i3c::sys::i2c::I2CPacket>> queue)
    : socket_(std::move(socket)),m_queue(queue)
  {
  }

  void Session::start()
  {
    do_read();
  }

  
typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

// stolen from http://dev.krzaq.cc/switch-on-strings-with-c11/
constexpr hash_t hash(char const* str, hash_t last_value = basis)
{
  return *str ? hash(str+1, (*str ^ last_value) * prime) : last_value;
}
constexpr unsigned long long operator "" _hash(char const* p, size_t)
{
  return hash(p);
}

i3c::sys::i2c::I2COperation strtoOp(std::string strop) {

  switch(hash(strop.c_str())){
    case "READ_SIMPLE"_hash:  return i3c::sys::i2c::I2COperation::READ_SIMPLE;
    case "WRITE_SIMPLE"_hash:  return i3c::sys::i2c::I2COperation::WRITE_SIMPLE;
    case "READ_REG_8"_hash: return i3c::sys::i2c::I2COperation::READ_REG_8;
    case "READ_REG_16"_hash: return i3c::sys::i2c::I2COperation::READ_REG_16;
    case "WRITE_REG_8"_hash: return i3c::sys::i2c::I2COperation::WRITE_REG_8;
    case "WRITE_REG_16"_hash: return i3c::sys::i2c::I2COperation::WRITE_REG_16;
  }
}



std::vector<i3c::sys::i2c::I2CPacket> genpacket ( const std::string s_config )
{
    std::vector <i3c::sys::i2c::I2CPacket> packets;


    libconfig::Config cfg;


    try {
        cfg.readString ( s_config );
    } catch ( libconfig::ParseException &pe ) {
        std::cout << pe.what() << ": " <<  pe.getError() << " on line " << pe.getLine() << ": " << pe.getError() << std::endl;
    }

    try {
        const libconfig::Setting& s_packets = cfg.getRoot() ["packets"];

        const int count = s_packets.getLength();

        for ( int i = 0; i < count; ++i ) {

            i3c::sys::i2c::I2COperation op;
            int reg=0;
            int data=0;
            std::string strop;
            int seqNo=0;
            const libconfig::Setting &packet = s_packets[i];
            int addressdata=0;
            if ( ! ( packet.lookupValue ( "I2CAddress", addressdata )
                     && packet.lookupValue ( "I2COperation", strop )
// 	    && packet.lookupValue ( "Register", reg )
                     && packet.lookupValue ( "Sequence", seqNo )
                     && packet.lookupValue ( "Data", data )
                   ) ) {
                std::cout << "something broke" << std::endl;
                continue;
            }

//             seqNo = configextract ( &cfg, "Sequence" );
            i3c::sys::i2c::I2CAddress peer ( addressdata );
	    op = strtoOp(strop);
	    if (   packet.lookupValue ( "Register", reg ) ) {
// 	     std::cout << "hier drin" << reg << " op: " << strop << std::endl;
                packets.push_back ( i3c::sys::i2c::I2CPacket ( seqNo, peer, op, reg, data ) );
            } else {
// 	      std::cout << "da drin" << reg << " op: " << strop << std::endl;
                packets.push_back ( i3c::sys::i2c::I2CPacket ( seqNo, peer, op,  data ) );
            }
        }
    } catch ( const libconfig::SettingNotFoundException &nfex ) {
        std::cout << "parsing the string did not work. check the data for flaws." <<std::endl;
    }
    return packets;
}

  void Session::parse() {
    std::vector<i3c::sys::i2c::I2CPacket> packets = genpacket(data_);
    std::cout << "successfully interpreted packet data" << std::endl;

    for (auto i: packets) { 
         m_queue.push_back(std::make_shared<i3c::sys::i2c::I2CPacket>(i));
	 std::cout << "added i2cpacket to working queue" << std::endl;
    }
  }
  
  void Session::do_read()
  {
    auto self(shared_from_this());
//     socket_.async_read_some(boost::asio::buffer(data_, max_length),
//         [this, self](boost::system::error_code ec, std::size_t length)
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {	  
          if (!ec)
          {
	    parse();
            do_write(parsesuccess_);
          }
        });
  }

  void Session::do_write(bool success)
  {
    int length = 3;
    std::string data;
    if (success)
    {
      data = "ACK";
    }
      else
      {
	data = "ERR";
      }
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
        });
  }


