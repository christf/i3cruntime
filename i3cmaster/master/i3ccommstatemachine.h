#pragma once
/*
 * I3C Communication State Machine
 */

#include <stdexcept>
#include <memory>
#include <vector>

#include "operation.h"
#include "i3cpacket.h"
#include "../sys/i2c/i2cpacket.h"

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
    I3CCommStateMachine(const I2CAddress& p_peer,
			const Operation& p_operation) noexcept;

    ~I3CCommStateMachine() noexcept = default;

    //! get the current state
    const CommState getState() const noexcept;

    //! return the packet counter, i.e. the total number of packets sent/received
    uint16_t getPacketCount() const throw();

    //! get the response. Throws an exception if the state is not FINISHED
    const std::vector<uint8_t>& getResponse() const throw(std::exception);

    //! get the error. Returns 0 if there is no error, check state for FAILED
    uint8_t getError() const noexcept;

    //! get the next I2C request
    std::shared_ptr<i2c::sys::I2CPacket> nextI2cRequest() throw(std::logic_error);

    //! process the I2C response
    void processI2cResponse(const i2c::sys::I2CPacket packet) throw(std::logic_error);

private:
    const I2CAddress m_peer;
    const Operation m_operation;

private:
    uint16_t m_packetCounter;
    CommState m_state;
    std::shared_ptr<i2c::sys::I2CPacket> m_lastPacket;
    std::shared_ptr<i2c::sys::I2CPacket> m_nextPacket;
    uint8_t m_frameProgress;
};

} // namespace master
} // namespace i3c

// End of file
