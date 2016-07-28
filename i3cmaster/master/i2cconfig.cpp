#include "i2cconfig.h"
  
  I2CMConfig::I2CMConfig(std::string filename) {
    libconfig::Config cfg;
    // Read the file. If there is an error, report it and exit.
    try {
        cfg.readFile ( "i2cmaster.cfg" );

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

	 
	std::shared_ptr<i3c::master::I3CEndpoint> epp = std::make_shared<i3c::master::I3CEndpoint> ( adr,eprio ) ;
	m_i3cendpoints.push_back( epp );
      }
    } catch ( const FileIOException &fioex ) {
        std::cerr << "I/O error while reading file." << std::endl;
    } catch ( const ParseException &pex ) {
        std::cerr << "Parse error " << pex.what() << " while reading " << pex.getFile() << " on line: " << pex.getLine()
                  << " - " << pex.getError() << std::endl;
    } catch ( const SettingNotFoundException &nfex ) {
      cerr << "error while parsing the config file" << endl;
      nfex.what();
      nfex.getPath();
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
