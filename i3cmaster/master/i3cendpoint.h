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

#ifndef I3CENDPOINT_H__
#define I3CENDPOINT_H__

#include <stdexcept>
#include <string>
#include <inttypes.h>
#include <map>
#include "../sys/i2c/i2cendpoint.h"
#include <memory>
#include "operation.h"
#include <iostream>
#include <bitset>

using namespace i2c::sys;
namespace i3c
{
namespace master 
{
  enum class endpoint_priority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    REALTIME = 3
  };

  static endpoint_priority stringToEnum(std::string input) throw (std::runtime_error) {
    if (input.compare("REALTIME") == 0)
      return endpoint_priority::REALTIME;
    if (input.compare("HIGH") == 0)
      return endpoint_priority::HIGH;
    if (input.compare("MEDIUM") == 0 )
      return endpoint_priority::MEDIUM;
    if (input.compare("LOW") == 0 )
      return endpoint_priority::LOW;

    throw(std::runtime_error("could not parse buspriority. Use one of REALTIME, HIGH, MEDIUM, LOW"));
  }
  
  //! I3C communication endpoint
  /*!
   * I3C endpoint representing a single device on the bus.
   *
   * This introduces an INT-Line in addition to device priorities
   */
  
  class I3CEndpoint 
  {
  public:
    
    
    //! Create an endpoint instance for a specific address.
    /*!
     * @param address The I2C address of the target device.
     * @throws std::out_of_range if the address is not a valid I2C address
     * @throw I2CEndpointException if the endpoint cannot be initialized
     */
    
    I3CEndpoint ( I2CAddress address, endpoint_priority priority ) throw ( I2CEndpointException ) ;
    
    ~I3CEndpoint() throw();
    
   
    //! Simple device read_reg_16
    /*!
     * Some devices present data when you read them without having to do any register transactions.
     *
     * @returns Result from the I2C call.
     * @throws i2c::I2CEndpointException if access to the I2C device fails.
     */
    uint8_t read() throw ( I2CEndpointException );
    //! Simple device write.
    /*!
     * Some devices accept data this way without needing to access any internal registers.
     *
     * @param data The data to be written.
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
    uint8_t write ( Operation operation ) throw ( I2CEndpointException );
    
    //! Read 8 bits of data from a device register.
    /*!
     * @param reg The device register.
     * @returns Result from the I2C call.
     * @throws I2CEndpointException if access to the I2C device fails.
     */
     uint8_t read ( const int reg ) throw ( I2CEndpointException );

     //! for easy printing
     friend std::ostream& operator<< ( std::ostream &out, const I3CEndpoint &endpoint );

  protected:
    int _fd() const throw();

  private:
    // No Copies of this instance!
    // (Not implemented and never to be called.)
    I3CEndpoint ( const I3CEndpoint& other );
    //! Write 8 bits of data to a device register.
    
//     const i2c::I2CEndpoint *m_endpoint;
   const I2CAddress m_address;
    
    int m_count; // this is increased if the device was the one to have activated the INT-Line 
    int m_packetcounter; // packet counter
    endpoint_priority m_bus_priority; // this together with count defines the order of polling when INT is active
  };

}
}
#endif
