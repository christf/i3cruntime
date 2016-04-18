#pragma once
/*
 * I3C Communication State Machine
 */

#include <stdexcept>
#include <vector>

#include "operation.h"
#include "../sys/i2c/i2cpacket.h"

using i3c::sys::i2c::I2CPacket;

namespace i3c {
namespace master {

class I3CCommStateMachine
{
public:
    // the internal state
    enum class CommState {
	//! The state is undefined. This should not happen.
	UNDEFINED = 0,
	//! An I2C packet is available for delivery
	I2C_PACKET_READY,
	//! An i2c packet is excpected as input
	I2C_PACKET_EXPECTED,
	//! The communication is finished
	FINISHED,
	//! The communication has failed and cannot be recovered
	FAILED
    };
public:
    //! Initialize the state machine with an operation
    I3CCommStateMachine(const Operation m_operation);

    //! get the current state
    const CommState state() const throw();

    //! get the response. Throws an exception if the state is not FINISHED
    const std::vector<uint8_t>& response() const throw(std::exception);

    //! get the error. Returns 0 if there is no error, check state for FAILED
    uint8_t error() const throw();

    //! return the packet counter, i.e. the total number of packets sent/received
    uint8_t packetCount() const throw();

    //! get the next I2C request
    I2CPacket nextI2cRequest() throw();

    //! process the I2C response
    void processI2cResponse(const I2CPacket packet) throw();
};

} // namespace master
} // namespace i3c

// End of file