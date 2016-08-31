#include "i2cconfig.h"
  
  I2CMConfig::I2CMConfig(std::string filename) {
    libconfig::Config cfg;
    // Read the file. If there is an error, report an d abort
    try {
        cfg.readFile ( filename.c_str() );

        m_i2cport  = (int)cfg.lookup ( "i2cport");
	cfg.lookupValue ( "name", m_name );
	cfg.lookupValue ( "bus", m_bus );
	 const Setting& root = cfg.getRoot();
	 const Setting &i2c = root["endpoints"]["i2c"];
      int count = i2c.getLength();

      for ( int i = 0; i < count; ++i ) {
	const Setting &i2ce = i2c[i];

	// Only output the record if all of the expected fields are present.
	string name ;
	//      uint8_t address;
	int address;
	string prio;
	enum i3c::master::endpoint_priority eprio;
	if ( ! ( i2ce.lookupValue ( "name", name )
	  && i2ce.lookupValue ( "address", address )
	  && i2ce.lookupValue ( "buspriority", prio )
	) ) {
	  continue;
	}
	eprio = i3c::master::stringToEnum ( prio );
	const I2CAddress adr ( address );

	m_i3cendpoints.push_back( std::make_shared<i3c::master::I3CEndpoint> ( adr,eprio ) );
      }
    } catch ( const FileIOException &fioex ) {
        std::cerr << "I/O error while reading file." << filename << std::endl;
	throw;
    } catch ( const ParseException &pex ) {
        std::cerr << "Parse error " << pex.what() << " while reading " << pex.getFile() << " on line: " << pex.getLine()
                  << " - " << pex.getError() << std::endl;
		  throw;
    } catch ( const SettingNotFoundException &nfex ) {
      cerr << "error while parsing the config file" << endl;
      nfex.what();
      nfex.getPath();
      throw;
    }
  }
  
  string I2CMConfig::getBus() {
    return m_bus;
  }
  string I2CMConfig::getName() {
    return m_name;
  }
  uint16_t I2CMConfig::getport() {
    return m_i2cport;
  }
 
deque< shared_ptr< i3c::master::I3CEndpoint > > I2CMConfig::getEndpoints()
{
return m_i3cendpoints;
}
