#include "connection.h"


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
using namespace i3c::sys::i2c;

I2COperation strtoOp(std::string strop) {

  switch(hash(strop.c_str())){
    case "READ_SIMPLE"_hash:  return I2COperation::READ_SIMPLE;
    case "WRITE_SIMPLE"_hash:  return I2COperation::WRITE_SIMPLE;
    case "READ_REG_8"_hash: return I2COperation::READ_REG_8;
    case "READ_REG_16"_hash: return I2COperation::READ_REG_16;
    case "WRITE_REG_8"_hash: return I2COperation::WRITE_REG_8;
    case "WRITE_REG_16"_hash: return I2COperation::WRITE_REG_16;
  }
}



std::vector<I2CPacket> genpacket ( const std::string s_config )
{

    std::vector <I2CPacket> packets;


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

            I2COperation op;
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
            I2CAddress peer ( addressdata );
	    op = strtoOp(strop);
	    if (   packet.lookupValue ( "Register", reg ) ) {
// 	     std::cout << "hier drin" << reg << " op: " << strop << std::endl;
                packets.push_back ( I2CPacket ( seqNo, peer, op, reg, data ) );
            } else {
// 	      std::cout << "da drin" << reg << " op: " << strop << std::endl;
                packets.push_back ( I2CPacket ( seqNo, peer, op,  data ) );
            }
        }
    } catch ( const libconfig::SettingNotFoundException &nfex ) {
        std::cout << "parsing the string did not work. check the data for flaws." <<std::endl;
    }
    return packets;
}


Connection::Connection ( int fd, std::vector<Connection> *clist ) :m_server_fd ( fd ),m_clist(clist)
{
    char ipstr[INET6_ADDRSTRLEN];
    int port;

    int new_sd;
    struct sockaddr_storage remote_info ;
    socklen_t addr_size;

    addr_size = sizeof ( addr_size );
    new_sd = accept ( m_server_fd, ( struct sockaddr * ) &remote_info, &addr_size );
    //TODO if (new_sd < 0) throw some error

    getpeername ( new_sd, ( struct sockaddr* ) &remote_info, &addr_size );

    // deal with both IPv4 and IPv6:
    if ( remote_info.ss_family == AF_INET ) {
        struct sockaddr_in *s = ( struct sockaddr_in * ) &remote_info;
        port = ntohs ( s->sin_port );
        inet_ntop ( AF_INET, &s->sin_addr, ipstr, sizeof ipstr );
    } else { // AF_INET6
        struct sockaddr_in6 *s = ( struct sockaddr_in6 * ) &remote_info;
        port = ntohs ( s->sin6_port );
        inet_ntop ( AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr );
    }

    std::cout << "Connection accepted from "  << ipstr <<  " using port " << port << std::endl;
    m_sd = new_sd;
}


const void Connection::run ( )
/// This function runs in a thread for every client, and reads incoming data.
/// It also writes the incoming data to all other clients.
{
    //   int rfd;

    char buf[1024];
    int buflen;
    int wfd;

    for ( ;; ) {
        //read incoming message.
        buflen = read ( m_sd, buf, sizeof ( buf ) );
        if ( buflen <= 0 ) {
            std::cout << "client disconnected. Cleaning fd. " << m_sd << std::endl ;
            close ( m_sd );
	    // TODO FIXME is vector really the correct data type? iterators are invalidated if the
// 	    m_clist->erase(m_myindex);
            return;
        } else {
            // TODO: do something useful with the data that was received
            std::string strbuf ( buf, buflen );
            std::cout << "reading data from client " << m_sd << ": " << strbuf;
	    do_command (  genpacket ( strbuf ) ) ;
        }
        /*
         *            // send the data to the other connected clients
         *            pthread_mutex_lock(&mutex_state);
         *
         *            for (wfd = 3; wfd < MAXFD; ++wfd)
         *            {
         *              if (FD_ISSET(wfd, &the_state) && (rfd != wfd))
         *              {
         *        	// add the users FD to the message to give it an unique ID.
         *        	string userfd;
         *        	stringstream out;
         *        	out << wfd;
         *        	userfd = out.str();
         *        	userfd = userfd + ": ";
         *        	send(wfd, userfd);
         *        	send(wfd, buf);
        }
        }

        pthread_mutex_unlock(&mutex_state);*/


    }
    //     return NULL;
}

void  Connection::do_command ( std::vector<I2CPacket> packets ) // function that will handle commands
{

  for ( std::vector<I2CPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i ) {
    std::cout << *i << ' ';
  }
}

