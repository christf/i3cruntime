#include "connection.h"


using namespace i3c::sys::i2c;


/*
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

        pthread_mutex_unlock(&mutex_state);


    }
    //     return NULL;
}

void  Connection::do_command ( std::vector<I2CPacket> packets ) // function that will handle commands
{

  for ( std::vector<I2CPacket>::const_iterator i = packets.begin(); i != packets.end(); ++i ) {
    std::cout << *i << ' ';
  }
}
*/
