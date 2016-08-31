/*
 * I3C Communication State Machine
 */

#include "i3ccommstatemachine.h"

#include "../api/i3c.h"
#include "i3cpacket.h"

using namespace i3c::sys;

namespace i3c {
namespace master {

I3CCommStateMachine::I3CCommStateMachine(const I2CAddress& p_peer,
					 const Operation& p_operation) noexcept
: m_peer(p_peer),
  m_operation(p_operation),
  m_packetCounter(0),
  m_state(CommState::I2C_PACKET_READY),
  m_lastPacket(nullptr),
  m_frameProgress(0)
{}

const I3CCommStateMachine::CommState I3CCommStateMachine::getState() const noexcept
{
  return m_state;
}

uint16_t I3CCommStateMachine::getPacketCount() const noexcept
{
  return m_packetCounter;
}

std::shared_ptr<I2CPacket> I3CCommStateMachine::nextI2cRequest() throw(std::logic_error)
{
  if (m_state != CommState::I2C_PACKET_READY)
    throw std::logic_error("Not in state Packet Ready!");

  std::shared_ptr<I2CPacket> packet = m_nextPacket;

  if (packet == nullptr) {

    // header packet if frame progress counter is zero
    if (m_frameProgress == 0) {
      I3CPacket header(m_operation.opcode(),
		      m_peer,
		      (packetcounter)(m_frameProgress%2+1),
		      i3c_packet_state::ST_START);

      packet = std::make_shared<I2CPacket>(header.render());
    } // frame progress == 0

    // operation parameters
    if (
      (m_frameProgress > 0 &&
      (m_frameProgress < m_operation.params().size()))
    ) {
      I3CPacket param(m_operation.params().at(m_frameProgress-1),
		      m_peer,
		      (packetcounter)(m_frameProgress%2+1),
		      i3c_packet_state::ST_ACK);

      packet = std::make_shared<I2CPacket>(param.render());
    } // parameters

    // response
    if (m_frameProgress >= m_operation.params().size()) {

    } // response
  }

  // we're expecting an answer now
  m_state = CommState::I2C_PACKET_EXPECTED;

  return packet;
}


void I3CCommStateMachine::processI2cResponse(const I2CPacket packet) throw(std::logic_error)
{
  if (m_state != CommState::I2C_PACKET_EXPECTED)
    throw std::logic_error("Not in state Packet Expected!");

  std::shared_ptr<I3CPacket> i3cp = nullptr;

  try {
    i3cp = std::make_shared<I3CPacket>(0, packet.data());
    // packet is okay, progress the frame
    ++m_frameProgress;
  } catch (std::invalid_argument) {
    // packet is invalid, keep i3cp at nullprt
  }
  // evaluate the contents
  if (i3cp == nullptr) {
      I3CPacket error(i3c_error::E_CRC,
		      m_peer,
		      packetcounter::ODD,
		      i3c_packet_state::ST_ERROR);

      // request to resend the packet
      m_nextPacket = std::make_shared<I2CPacket>(error.render());
  }

  // new request is ready
  m_state = CommState::I2C_PACKET_READY;
}


} // namespace master
} // namespace i3c

// End of file
