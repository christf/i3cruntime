/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "i3cendpoint.h"
#include "i3cpacket.h"
namespace i3c
{
namespace master
{
// I3CEndpoint::I3CEndpoint ( const I3CEndpoint& other ) : I3CEndpoint()
// {
//
// }

uint8_t I3CEndpoint::read ( const int reg ) throw ( I2CEndpointException )
{
// TODO implementieren
}

uint8_t I3CEndpoint::write ( Operation operation ) throw ( I2CEndpointException )
{
// TODO implementieren

    // 1. paket aus den daten bauen
    // 2. paket an den endpoint via i2c übertragen

}

// TODO use packetcounter enum
I3CEndpoint::I3CEndpoint ( I2CAddress address, enum endpoint_priority priority ) throw ( I2CEndpointException )
: m_address(address),
m_count ( 0 ),m_packetcounter ( 0 ) ,
m_bus_priority ( priority )
{
    // TODO implementieren
// //   m_i2c_endpoint = i
}

std::ostream& operator<< ( std::ostream &out, const I3CEndpoint &endpoint )
{
  std::bitset<8> adr ( endpoint.m_address );
  std::bitset<8> pc ( endpoint.m_count );
  // std::bitset<8> prio ( endpoint.m_bus_priority );
  out << "[ address: " << std::hex << unsigned ( endpoint.m_address )  << "(" << adr << ") PacketCounter: "  << unsigned ( endpoint.m_count ) <<
  " (" << pc << ") priority: " << unsigned ( endpoint.m_bus_priority ) << "]" << std::endl;
  return out;
}

I3CEndpoint::~I3CEndpoint() throw()
{

}



}
}
