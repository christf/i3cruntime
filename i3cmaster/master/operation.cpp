#include "operation.h"

namespace i3c {
namespace master {

Operation::Operation( uint8_t operation, std::vector<uint8_t> params)
: operation(operation), m_data(params)
{


}

} // namespace master
} // namespace i3c
