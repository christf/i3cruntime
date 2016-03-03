/*
 * * i3c Master-Implementation
 **
 ** Author: Christof Schulze <christof.schulze@gmx.net>
 */

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "i3c_master.h"

#include <unistd.h>


#include <string.h>

#include <iostream>
// #include "slave.h"
// #include "i3c_commhandler.h"

#include "i3cpacket.h"


#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include "operation.h"
#include "../api/i3c.h"

#include <libconfig.h++>

using namespace std;



template<typename T> void print_queue ( T& q )
{
    while ( !q.empty() ) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

#include "../debug.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <iostream>
#include <cstring>      // Needed for memset
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions


#include "../server.h"
#include "../master/i3cendpoint.h"
using namespace libconfig;

using namespace i3c::sys::i2c;


void sclient()
{
    // socket client implementation taken from http://easy-tutorials.net/c/linux-c-socket-programming/
    int status;
    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.

    // The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
    // to by hints must contain either 0 or a null pointer, as appropriate." When a struct
    // is created in C++, it will be given a block of memory. This memory is not necessary
    // empty. Therefor we use the memset function to make sure all fields are NULL.
    memset ( &host_info, 0, sizeof host_info );

    std::cout << "Setting up the structs..."  << std::endl;

    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.

    // Now fill up the linked list of host_info structs with google's address information.
    status = getaddrinfo ( "www.google.com", "80", &host_info, &host_info_list );
    // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    //       std::cout << status;
    if ( status != 0 ) {
        std::cout << "getaddrinfo error" << gai_strerror ( status )  << endl;
    } else {
        std::cout << "Creating a socket..."  << std::endl;
        int socketfd ; // The socket descripter
        socketfd = socket ( host_info_list->ai_family, host_info_list->ai_socktype,
                            host_info_list->ai_protocol );
        if ( socketfd == -1 ) {
            std::cout << "socket error " ;
        }

        std::cout << "Connect()ing..."  << std::endl;
        status = connect ( socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen );
        if ( status == -1 ) {
            std::cout << "connect error" ;
        }
        std::cout << "send()ing message..."  << std::endl;
        char msg[255] = "GET / HTTP/1.1\nhost: www.google.com\n\n";
        int len;
        ssize_t bytes_sent;
        len = strlen ( msg );
        bytes_sent = send ( socketfd, msg, len, 0 );
        std::cout << "Waiting to recieve data..."  << std::endl;
        ssize_t bytes_recieved;
        char incoming_data_buffer[1000];
        bytes_recieved = recv ( socketfd, incoming_data_buffer,1000, 0 );
        // If no data arrives, the program will just wait here until some data arrives.
        if ( bytes_recieved == 0 ) {
            std::cout << "host shut down." << std::endl ;
        }
        if ( bytes_recieved == -1 ) {
            std::cout << "recieve error!" << std::endl ;
        }
        std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
        std::cout << incoming_data_buffer << std::endl;
        std::cout << "Receiving complete. Closing socket..." << std::endl;
        freeaddrinfo ( host_info_list );
        close ( socketfd );
    }
}



int server_func ( int socketfd )
{
    int new_sd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof ( their_addr );
    new_sd = accept ( socketfd, ( struct sockaddr * ) &their_addr, &addr_size );
    if ( new_sd == -1 ) {
        std::cout << "listen error" << std::endl ;
    } else {
        std::cout << "Connection accepted. Using new socketfd : "  <<  new_sd << std::endl;
    }


    std::cout << "Waiting to recieve data..."  << std::endl;
    ssize_t bytes_recieved;
    char incomming_data_buffer[1000];
    bytes_recieved = recv ( new_sd, incomming_data_buffer,1000, 0 );
// If no data arrives, the program will just wait here until some data arrives.
    if ( bytes_recieved == 0 ) {
        std::cout << "host shut down." << std::endl ;
    }
    if ( bytes_recieved == -1 ) {
        std::cout << "recieve error!" << std::endl ;
    }
    std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
    incomming_data_buffer[bytes_recieved] = '\0';
    std::cout << incomming_data_buffer << std::endl;


    std::cout << "send()ing back a message..."  << std::endl;
    char msg[255] = "thank you.";
    int len;
    ssize_t bytes_sent;
    len = strlen ( msg );
    bytes_sent = send ( new_sd, msg, len, 0 );

    std::cout << "Stopping server..." << std::endl;

    close ( new_sd );
    close ( socketfd );
}





int main()
{
    Config cfg;
    int i3cport;
    // Read the file. If there is an error, report it and exit.
    try {
        cfg.readFile ( "i2cmaster.cfg" );
    } catch ( const FileIOException &fioex ) {
        std::cerr << "I/O error while reading file." << std::endl;
        return ( EXIT_FAILURE );
    } catch ( const ParseException &pex ) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return ( EXIT_FAILURE );
    }

    // Get the store name.
    try {
        string name = cfg.lookup ( "name" );
        cout << "Bus name: " << name << endl << endl;
    } catch ( const SettingNotFoundException &nfex ) {
        cerr << "No 'name' setting in configuration file." << endl;
    }
    // Get the store name.
    try {
        i3cport = (int)cfg.lookup ( "i3cport" );
    } catch ( const SettingNotFoundException &nfex ) {
        cerr << "No 'i3cport' setting in configuration file." << endl;
    }
    // Get the store name.
    string bus;
    try {
        string busdev = cfg.lookup ( "bus" );
        bus = busdev;
        cout << "Bus : " << bus << endl << endl;
    } catch ( const SettingNotFoundException &nfex ) {
        cerr << "No 'bus' setting in configuration file." << endl;
    }


// TODO - socket öffnen um Daten entgegen zu nehmen. Diese Daten per i2c über den Bus schicken.
// TODO initialize the i2c-bus,


    sclient();
    std::cout << " ========= server =========" << endl;
    Server serv(i3cport);
    serv.run();

    uint8_t destination = 0x21;
    uint8_t data = 0xaa;
    I3CPacket p ( data ,destination, ODD, ST_START );

    cout << p << endl;



    const Setting& root = cfg.getRoot();

    // Output a list of all i3c in the endpoints.
    try {
        const Setting &i3c = root["endpoints"]["i3c"];
        int count = i3c.getLength();

        cout << setw ( 30 ) << left << "NAME" << "  "
             << setw ( 10 ) << left << "ADDRESS" << "   "
             << setw ( 6 ) << left << "PRIO" << "  "
             << endl;

        for ( int i = 0; i < count; ++i ) {
            const Setting &i3ce = i3c[i];

            // Only output the record if all of the expected fields are present.
            string name ;
            //      uint8_t address;
            int address;
            string prio;
	    enum i3c::master::endpoint_priority eprio;
            if ( ! ( i3ce.lookupValue ( "name", name )
                     && i3ce.lookupValue ( "address", address )
                     && i3ce.lookupValue ( "buspriority", prio )
                   ) ) {
                continue;
            }
            eprio = i3c::master::stringToEnum ( prio );
            const i3c::sys::i2c::I2CAddress adr ( address );

	    const i3c::master::I3CEndpoint ep ( adr,eprio )  ;
            cout << setw ( 30 ) << left << name << "  "
                 << setw ( 10 ) << left << address << "  "
                 << setw ( 6 ) << left << unsigned ( eprio ) << "  "
                 << endl;
        }
        cout << endl;
    } catch ( const SettingNotFoundException &nfex ) {
        // Ignore
    }

    // Output a list of all i3c in the endpoints.
    try {
        const Setting &i2c = root["endpoints"]["i2c"];
        int count = i2c.getLength();

        cout << setw ( 30 ) << left << "NAME" << "  "
             << setw ( 10 ) << left << "ADDRESS" << "   "
             << setw ( 6 ) << left << "PRIO" << "  "
             << endl;

        for ( int i = 0; i < count; ++i ) {
            const Setting &i2ce = i2c[i];

            // Only output the record if all of the expected fields are present.
            string name, media;
            string prio;
            int address;

            if ( ! ( i2ce.lookupValue ( "name", name )
                     && i2ce.lookupValue ( "buspriority", prio )
                     && i2ce.lookupValue ( "address", address )
                   ) ) {
                continue;
            }

            cout << setw ( 30 ) << left << name << "  "
                 << setw ( 10 ) << left << address << "  "
                 << setw ( 6 ) << left << prio << "  "
                 << endl;
        }
        cout << endl;
    } catch ( const SettingNotFoundException &nfex ) {
        // Ignore.
    }

    return ( EXIT_SUCCESS );



//  //   printf ( "%s\n", result);
//     std::priority_queue<int> q;
//
//     for ( int n : {
//                 1,8,5,6,3,4,0,9,3,2
//             } )
//         q.push ( n );
//
//     print_queue ( q );
//
//
//
//     std::priority_queue<int, std::vector<int>, std::greater<int> > q2;
//
//     for ( int n : {
//                 1,8,5,6,3,4,0,9,3,2
//             } )
//         q2.push ( n );
//
//     print_queue ( q2 );
}
