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
#include "i3cpacket.h"
#include <functional>
#include <queue>
#include <vector>
#include "operation.h"
#include "../api/i3c.h"

#include "../debug.h"
#include <iomanip>
#include <cstdlib>
#include <memory>
#include <utility>
#include "../server.h"

using namespace std;





#include "../master/i3cendpoint.h"
#include "i2cconfig.h"

using namespace i3c::sys::i2c;




template<typename T> void print_queue ( T& q )
{
    while ( !q.empty() ) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

int main()
{
  I2CMConfig i2cconfig("i2cmaster.cfg");
// cout << i2cconfig.getBus() << i2cconfig.getName() << i2cconfig.getport() << endl;

// print_queue(i2cconfig.getEndpoints());
    
    std::deque<int> q;

    for ( int n : {
                1,8,5,6,3,4,0,9,3,2
            } )
        q.push_back ( n );

    while ( !q.empty() ) {
        std::cout << q.at(0) << " ";
         q.pop_front();
    }
    std::cout << '\n';


// TODO - socket öffnen um Daten entgegen zu nehmen. Diese Daten per i2c über den Bus schicken.
// TODO initialize the i2c-bus,


  try
  {
    boost::asio::io_service io_service;

    Server s(io_service, i2cconfig.getport());

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
