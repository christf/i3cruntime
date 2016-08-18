#pragma once

#include <inttypes.h>
#include <iostream>
#include <queue>
#include "../master/i3cendpoint.h"
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

class I2CMConfig {
public:
  I2CMConfig(std::string filename);
  uint16_t getport();
  string getName();
  string getBus();
  std::deque<std::shared_ptr<i3c::master::I3CEndpoint>> getEndpoints();
private:
  uint16_t m_i2cport;
  std::string m_name;
  std::string m_bus;
  
  std::deque<std::shared_ptr<i3c::master::I3CEndpoint>> m_i3cendpoints;
};