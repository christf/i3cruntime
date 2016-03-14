#include "server.h"


Server::Server ( int i3cport ) : connections()
{
    struct addrinfo hostinfo, *res;

    std::string strport =  std::to_string ( i3cport );
    /// socket server implementation
    int status;
    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.

    // The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
    // to by hints must contain either 0 or a null pointer, as appropriate." When a struct
    // is created in c++, it will be given a block of memory. This memory is not nessesary
    // empty. Therefor we use the memset function to make sure all fields are NULL.
    memset ( &host_info, 0, sizeof host_info );
    // first, load up address structs with getaddrinfo():


    std::cout << "starting server on port " << i3cport  << std::endl;

    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

    // Now fill up the linked list of host_info structs with google's address information.

    status = getaddrinfo ( NULL, strport.c_str(), &host_info, &host_info_list );
    // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    if ( status != 0 ) {
        std::cout << "getaddrinfo error" << gai_strerror ( status ) ;
    }


    std::cout << "Creating a socket..."  << std::endl;
    int socketfd ; // The socket descripter
    socketfd = socket ( host_info_list->ai_family, host_info_list->ai_socktype,
                        host_info_list->ai_protocol );
    if ( socketfd == -1 ) {
        std::cout << "socket error " ;
    }


    std::cout << "Binding socket..."  << std::endl;
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code. (see man page for more information)
    int yes = 1;
    //prevent "Error Address already in use"
    status = setsockopt ( socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof ( int ) );
    if ( status < 0 ) {
        std::cout << "socket already in use" << std::endl ;
    }

    status = bind ( socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen );
    if ( status == -1 ) {
        std::cout << "bind error" << std::endl ;
    }

    std::cout << "Listen()ing for connections..."  << std::endl;
    status =  listen ( socketfd, 5 );
    if ( status == -1 ) {
        std::cout << "listen error" << std::endl ;
    }

    freeaddrinfo ( host_info_list );

    if ( socketfd == -1 ) {
        // TODO throw exception here
        std::cout << "An error occured. Closing program." ;
        throw std::runtime_error ( "could not open server socket" );
    }
    m_server_fd = socketfd;
}


// int Server::send ( Connection c, uint16_t data )
// // This function will send data to the clients fd.
// // data contains the message to be send
// {
//     int ret;
//
// // TODO  implement this function
// //  ret = send(fd, data.c_str(), strlen(data.c_str()),0);
//     //if(ret != strlen(data.c_str()) throw some error;
//     return 0;
// }





void Server::run()
{
    std::thread threads[MAXFD]; //create 10 handles for threads.

    while ( 1 ) { // start looping here
        int *arg;
        // if a client is trying to connect, establish the connection and create a fd for the client.

	if (connections.size() < MAXFD){
	  //TODO: guards
	  Connection c(m_server_fd);
	connections.push_back(c);
	// TODO: how to remove dead connections? => I could change the parameter for the constructor of Connections to a Server,
// 	// pass it by reference and call some member method to remove it from the queue. <- rampant layering violation alert.

//        server_send(rfd, welcome_msg); // send a welcome message/instructions.

            // now create a thread for this client to intercept all incoming data from it.
//             threads[rfd] ( &Server::tcp_read,  this);
// 	    threads[rfd] ( &Connection::run);
//             threads[rfd] ( &Connection::run, &c );
	    c.run();
//             threads[rfd].join();
//  	    threads[rfd] ( this->bla);
         }
    }
}


// void Server::bla() {
//   // TODO: fix this shit. in here connect.tcp_read showld be called one or the other way...
//   assignconnection->lock();
//   FD_CLR ( *rfd, &the_state );   // free fd's from  clients
//   assignconnection->unlock();
//   close ( *rfd );
//   pthread_exit ( NULL );
// }
