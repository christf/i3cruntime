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

class Connection
{
public:
        Connection ( int fd );
private:
        const int m_server_fd;

        int m_sd;
        std::mutex *assignconnection;
        void do_command ( char *command );


public:
        int establish();
        const void run( ) ;
};
