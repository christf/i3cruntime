#include "../sys/i2c/i2cendpoint.h"
#include "i3cendpoint.h"
#include <vector>

using namespace i3c::sys::i2c;


namespace i3c {
  namespace master {
//!  virtual class for i3c Endpointbroker which will run i3c on top of i2c. Please implement this in i3c::platform namespaces
class I3CEndpointBroker
{
  public:
    //! Clean-up the instance and clean-up/remove all existing I2C endpoints.
    virtual ~I3CEndpointBroker() throw();

    //! Create (if necessary) and return an I2C endpoint for the specified address.
    virtual std::shared_ptr<I3CEndpoint> endpoint (const I2CAddress address) throw (I2CEndpointException) = 0;

};
} // namespace master
} // namespace i3c