#include "i2cconfig.h"
  
  I2CMConfig::I2CMConfig(std::string filename) {
    libconfig::Config cfg;
    // Read the file. If there is an error, report it and exit.
    try {
        cfg.readFile ( filename.c_str() );

        m_i2cport  = (int)cfg.lookup ( "i2cport");
	cfg.lookupValue ( "name", m_name );
	cfg.lookupValue ( "bus", m_bus );
	 const Setting& root = cfg.getRoot();
	 const Setting &i3c = root["endpoints"]["i3c"];
      int count = i3c.getLength();

      for ( int i = 0; i < count; ++i ) {
	const Setting &i3ce = i3c[i];

	// Only output the record if all of the expected fields are present.
	string name ;
	//      uint8_t address;
	int address;
	string prio;
	enum i3c::master::endpoint_priority eprio;
	if ( ! ( i3ce.lookupValue ( "name", name )
	  && i3ce.lookupValue ( "address", address )
	  && i3ce.lookupValue ( "buspriority", prio )
	) ) {
	  continue;
	}
	eprio = i3c::master::stringToEnum ( prio );
	const i3c::sys::i2c::I2CAddress adr ( address );

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
