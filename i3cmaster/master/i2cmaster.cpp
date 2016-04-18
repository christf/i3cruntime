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
#include "i3cpacket.h"
#include <functional>
#include <queue>
#include <vector>
#include "operation.h"
#include "../api/i3c.h"
#include <libconfig.h++>
#include "../debug.h"
#include <iomanip>
#include <cstdlib>
#include <memory>
#include <utility>
#include "../server.h"

using namespace std;



template<typename T> void print_queue ( T& q )
{
    while ( !q.empty() ) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

#include "../master/i3cendpoint.h"


using namespace i3c::sys::i2c;





int main()
{
    Config cfg;
    int i2cport;
    // Read the file. If there is an error, report it and exit.
    try {
        cfg.readFile ( "i2cmaster.cfg" );
    } catch ( const FileIOException &fioex ) {
        std::cerr << "I/O error while reading file." << std::endl;
        return ( EXIT_FAILURE );
    } catch ( const ParseException &pex ) {
        std::cerr << "Parse error " << pex.what() << " while reading " << pex.getFile() << " on line: " << pex.getLine()
                  << " - " << pex.getError() << std::endl;

        return ( EXIT_FAILURE );
    }

    // Get the name of the server endpoint
    try {
        string name = cfg.lookup ( "name" );
        cout << "Bus name: " << name << endl << endl;
    } catch ( const SettingNotFoundException &nfex ) {
        cerr << "No 'name' setting in configuration file." << endl;
    }
    // Get the port of the i2c server
    try {
        i2cport = ( int ) cfg.lookup ( "i2cport" );
    } catch ( const SettingNotFoundException &nfex ) {
        cerr << "No 'i2cport' setting in configuration file." << endl;
    }
    // which i2c-device are we on?
    string bus;
    try {
        string busdev = cfg.lookup ( "bus" );
        bus = busdev;
        cout << "Bus : " << bus << endl << endl;
    } catch ( const SettingNotFoundException &nfex ) {
        cerr << "No 'bus' setting in configuration file." << endl;
    }



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


    
    
 //   printf ( "%s\n", result);
    std::priority_queue<int> q;

    for ( int n : {
                1,8,5,6,3,4,0,9,3,2
            } )
        q.push ( n );

    print_queue ( q );



    std::priority_queue<int, std::vector<int>, std::greater<int> > q2;

    for ( int n : {
                1,8,5,6,3,4,0,9,3,2
            } )
        q2.push ( n );

    print_queue ( q2 );

// TODO - socket öffnen um Daten entgegen zu nehmen. Diese Daten per i2c über den Bus schicken.
// TODO initialize the i2c-bus,


  try
  {
    boost::asio::io_service io_service;

    Server s(io_service, i2cport);

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

      return ( EXIT_SUCCESS );
  
  
  
//     std::cout << " ========= server =========" << endl;
//     Server serv ( i2cport );
//     serv.run();
// 
//     // TODO all of the below should be handled inside a connection.
//     uint8_t destination = 0x21;
//     uint8_t data = 0xaa;
//     I3CPacket p ( data ,destination, ODD, ST_START );
// 
//     cout << p << endl;




}
