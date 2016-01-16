
#include <stdint.h>
#include <stdbool.h>

enum packet_state {
	/*
	 * Es sind keine Fehler aufgetreten.
	 */
	ST_ACK = 0,

	/* (master only)
	 * Das Paket mit diesem Status leitet einen neuen Frame ein.
	 * Die Payload enth�lt den OpCode.
	 */
	ST_START = 1,

	/*
	 * Wird gesetzt, wenn die Parameter (vom Master zum Slave) oder die
	 * Antwort (vom Slave zum Master) vollst�ndig gesetzt sind.
	 */
	ST_FIN = 2,

	/*
	 * Es kam zu einem Problem bei der Daten�bertragung.
	 * Die Payload enth�lt einen detaillierten Fehlerstatus
	 * (siehe ERROR-Byte bzw. enum error).
	 */
	ST_ERROR = 3

	/*
	 * Packet state is coded by four bits in the packet header,
	 * there cannot be any additions to this enum without changing
	 * the protocol structure!
	 */
};

/*
 * Error status codes if ST states ERROR.
 */
enum error {
	/* Abort
	 *
	 * Abbruch der Kommunikation, Verwerfen des aktuellen Frames
	 */
	E_ABORT = 0,

	/* CRC-Fehler
	 *
	 * das letzte Paket wird erneut verschickt
	 */
	E_CRC = 1,

	/* Frame Error
	 *
	 * Der Frame wird aufgrund von Paketverlust verworfen,
	 * Der Master startet die �bertragung des Frames erneut.
	 */
	E_FRAME = 2,

	/* Unbekannter OpCode
	 *
	 * Der OpCode wurde nicht erkannt.
	 */
	E_OPCODE =3,

	/* FIFO Overrun
	 *
	 * Der Befehlspuffer im Slave ist noch belegt, derzeit k�nnen
         * keine Pakete empfangen werden.
	 */
	E_FIFO = 4,

	/* No Packet Available
	 *
	 * (Antwort auf 2) Es liegt kein Paket zum Re-Send vor.
	 * Dieser Fehler tritt auf, wenn der Slave einen Reset
	 * durchgef�hrt hat und der Master die letzte Antwort nochmals
	 * anfordert.
	 */
	E_NOPACKET = 5,

	/*
	 * Additional error codes can be added here.
	 */
};

/*
 * Reset Status information, stating if any why the slave has just come out
 * of a hardware reset.
 *
 * Wenn der Reset-Status sich auf einen Wert verschieden von 00 �ndert,
 * wird auch der INT-Status aktiviert. Der Reset-Status wird nach der
 * ersten Status-Abfrage zur�ckgesetzt.
 */
enum resetstatus {
        /*
	 * Normalbetrieb
	 */
	RST_NONE = 0,

	/*
	 * Reset (extern �ber den RESET-Pin oder per Power-On)
	 */
	RST_EXTERAL = 1,

	/*
	 * Software-Reset
	 */
	RST_SOFTWARE = 2,

	/*
	 * Watchdog-Reset
	 */
	RST_WATCHDOG = 3

	/*
	 * Reset state is coded by four bits in the status response,
	 * there cannot be any additions to this enum without changing
	 * the protocol structure!
	 */
};

enum opcode {
	/*
	 * Reset bewirkt einen Reboot des Clients.
	 */
	OP_RESET = 1,

	/*
	 * Statusabfrage (siehe Abschnitt "Statusbyte")
	 */
	OP_STATUS = 2,

	/*
	 * Register lesen
	 */
	OP_REGISTER_READ = 3,

	/*
	 * Register schreiben
	 */
	OP_REGISTER_WRITE = 4,

	/*
	 * Firmware-Update Management (mit Sub-Kommando etc im Frame)
	 */
	OP_FIRMWARE_MGMT = 5,

	/*
	 * Firmware-Update Daten
	 */
	OP_FIRMWARE_DATA = 6,

	/*
	 * Device-Descriptor auslesen
	 */
	OP_DEVICEDESCRIPTOR_READ = 7,

	/*
	 * I2C-Adresse setzen
	 */
	OP_DEVICEDESCRIPTOR_I2CSET = 8,

	/*
	 * Device-Descriptor (au�er I2C-Adresse) setzen
	 */
	OP_DEVICEDESCRIPTOR_WRITE = 9,

	/*
         * OpCodes 10 throuth 15 are reserved for protocol use.
	 * Application OpCodes must not be added here, but registered by the
	 * application.
	 */

	/*
	 * OpCodes 16 through 255 are application specific.
	 */
};


struct slave {
	uint8_t address[255];
};

enum packetcounter {
	EVEN = 0,
	ODD = 1
};

struct packet  {
	uint8_t data;
	enum packet_state status;
	enum packetcounter pc;
	uint8_t crc;
	uint8_t adr;
	uint8_t destination;
} ;

struct frame {
	// amount of packets inside frame
	int length;
	struct packet *packets;
} ;

/*
   1	0001
   2	0010
   3	0011
   4	0100
   5	0101
   6	0110
   7	0111
   8	1000
   9	1001
   A	1010
   B	1011
   C	1100
   D	1101
   E	1110
   F	1111

*/