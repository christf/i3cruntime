#ifndef SERVER_CLASS
#define SERVER_CLASS

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
#include "connection.h"
class Server
{
        const int MAXFD = 10;
//   const int MAXLEN = 1024;
private:

        void do_command ( char * command, int fd );
        int m_server_fd;
        volatile fd_set the_state;
        static std::mutex assignconnection;

public:
        Server ( int tcp_port );
        void run();
        int send ( Connection connection, uint16_t data );

};

#endif
