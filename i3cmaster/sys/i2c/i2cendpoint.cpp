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

#include "i2cendpoint.h"

namespace i3c {
namespace sys {
namespace i2c {

I2CEndpoint::I2CEndpoint(const I2CAddress address) throw (I2CEndpointException)
  : m_address(address)
{
}

I2CEndpoint::~I2CEndpoint()
{
}


const I2CAddress I2CEndpoint::address() const
{
  return m_address;
}


} // namespace i2c
} // namespace sys
} // namespace i3c