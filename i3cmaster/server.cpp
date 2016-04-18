#include "server.h"



  Server::Server(boost::asio::io_service& io_service, short port)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
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
            std::make_shared<Session>(std::move(socket_))->start();
          }

          do_accept();
        });
  }

  
  

  Session::Session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void Session::start()
  {
    do_read();
  }

  void Session::parse() {
        Config cfg;
	try {
	  cfg.readString(data_);
	  const Setting &packets = cfg.getRoot()["packets"];
	  int count = packets.getLength();
// 	  cout << count << endl;
	  
	  parsesuccess_ = true;
	  // TODO: wie kommen diese PAkete nun in die priorityqueue, deren Inhalt auf den I2C-Bus gelegt wird?
	}
	catch ( const ParseException &pex ) {
	  // const ParseException &pex 
	  // const SettingNotFoundException &nfex )
        std::cerr << "Parse error " << pex.what() << " while reading " << pex.getFile() << " on line: " << pex.getLine()
                  << " - " << pex.getError() << std::endl;
		  parsesuccess_ = false;
	}
	catch ( const SettingNotFoundException &nfex ) {
	  parsesuccess_ = false;
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


