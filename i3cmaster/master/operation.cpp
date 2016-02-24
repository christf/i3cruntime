#include "operation.h"

namespace i3c {
namespace master {

Operation::Operation( uint8_t opcode, std::vector<uint8_t> params)
: m_opcode(opcode), m_params(params)
{
}

const uint8_t Operation::opcode() const throw()
{
  return m_opcode;
}

const std::vector< uint8_t >& Operation::params() const throw()
{
  return m_params;
}

} // namespace master
} // namespace i3c
