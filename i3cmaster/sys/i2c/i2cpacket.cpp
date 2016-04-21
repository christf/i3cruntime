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


#include "i2cpacket.h"

namespace i3c {
namespace sys {
namespace i2c {

I2CPacket::I2CPacket(const uint16_t seqNo,
		     const I2CAddress peer,
		     const I2COperation op,
	             const uint16_t data) throw(std::invalid_argument)
: m_seqNo(seqNo), m_peer(peer), m_op(op), m_reg(0), m_data(data), m_priority(5)
{
  if ( (op != I2COperation::READ_SIMPLE) &&
       (op != I2COperation::WRITE_SIMPLE) )
    throw std::invalid_argument("Only operations READ_SIMPLE and WRITE_SIMPLE are allowed without register.");
}

I2CPacket::I2CPacket(const uint16_t seqNo,
		     const I2CAddress peer,
		     const I2COperation op,
		     const uint8_t reg,
	             const uint16_t data) throw(std::invalid_argument)
: m_seqNo(seqNo),
  m_peer(peer),
  m_op(op),
  m_reg(reg),
  m_data(data),
  m_priority(5)
{
  if ( (op == I2COperation::READ_SIMPLE) ||
       (op == I2COperation::WRITE_SIMPLE) )
    throw std::invalid_argument("Operations READ_SIMPLE and WRITE_SIMPLE are not allowed with a register.");
}

I2CPacket::I2CPacket(const I2CPacket& other, const uint16_t data) throw()
: m_seqNo(other.m_seqNo),
  m_peer(other.m_peer),
  m_op(other.m_op),
  m_reg(other.m_reg),
  m_data(data),
  m_priority(5)
{
}

// #include <hex>


// TODO: initialize m_priority!

std::ostream& operator<< ( std::ostream &out, const I2CPacket &packet )
{
  std::bitset<8> bdest ( packet.m_peer );
  std::bitset<8> bseq ( packet.m_seqNo );
//   std::bitset<8> bmeta = 0x00;
  std::bitset<8> bdata ( packet.m_data );
  std::bitset<8> breg ( packet.m_reg );
//   const uint16_t m_seqNo;
//   const I2CAddress m_peer;
//TODO   const I2COperation m_op;
//   const uint8_t m_reg;
//   const uint16_t m_data;


  out << "[ destination: " << std::hex << unsigned ( packet.m_peer )  << "(" << bdest << ") sequenz: "  << unsigned ( packet.m_seqNo ) <<
  " (" << bseq << ") data: " << unsigned ( packet.m_data ) << "(" << bdata  << ") register; " << unsigned(packet.m_reg ) << " (" << breg <<  ") operation: " << (int)packet.m_op << ") ]" << std::endl;
  return out;
}

bool operator< (const I2CPacket& lhs, const I2CPacket& rhs)
{
  return lhs.getprio() < rhs.getprio();
}

const uint8_t I2CPacket::getprio() const
{
return m_priority;
}


I2CPacket::I2CPacket(const I2CPacket &other) throw()
: m_seqNo(other.m_seqNo),
  m_peer(other.m_peer),
  m_op(other.m_op),
  m_reg(other.m_reg),
  m_data(other.m_data),
  m_priority(5)
{}

const uint16_t I2CPacket::seqNo() const throw()
{
  return m_seqNo;
}

const I2CAddress I2CPacket::peer() const throw()
{
  return m_peer;
}

const I2COperation I2CPacket::op() const throw()
{
  return m_op;
}

const uint8_t I2CPacket::reg() const throw()
{
  return m_reg;
}

const uint16_t I2CPacket::data() const throw()
{
  return m_data;
}

} // namespace i2c
} // namespace sys
} // namespace i3c
