#pragma once
#include <stdint.h>
#include "../api/i3c.h"
#include <iostream>


class i3cpacket
{

  /**
   * See http://users.ece.cmu.edu/~koopman/crc/ for details.
   *
   * The CRC polynomial is
   * 	0x12 => x^5 + x^2 + 1
   * 	which is 0x25 in explicit +1 notation,
   * 	then moved to the beginning of the byte
   */
  #define POLYNOMIAL (0x25 << 2)

private:
        uint8_t data;
        uint8_t meta;
        uint8_t destination;
	enum packetcounter packetcount;
        enum i3c_packet_state status;
      uint8_t crc;
public:
        void create ( uint8_t data, uint8_t destination, enum packetcounter pc, enum i3c_packet_state st );
	void interpret (uint8_t data, uint8_t metadata);

	//! check if the package contains a valid crc
	bool isvalid ();
	void tostr();
	friend std::ostream& operator<<(std::ostream &out, i3cpacket &packet);

private:
	uint8_t CRC5x12(uint8_t crc, uint8_t data);
};



