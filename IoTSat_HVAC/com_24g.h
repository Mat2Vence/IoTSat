/*
 * com_24g.h
 * Version 0.1 January, 2015
 * Copyright 2015 Mathieu THOMAS
 * For details, see http://www.emtee.fr
 *
 *
 * 
 * Communication through RF 2.4GHz using Payload
 * 
 * 
 *
 *
 *
 *
 */

#include <inttypes.h>


#include "nRF24L01.h"
#include "RF24_config.h"
#include "RF24.h"


#define COM_24G_VERSION 0.1;
#define PACK __attribute__((packed))  // Important declaration to make sure cross-platform compilation of structure are equivalent

// PayloadType: Enumeration of Payload Type to be managed
enum PayloadType {
  
  WHOAMI,                     // Command to request its identifier
  YOUARE,                     // Response to WHOAMI that give the information back
  IAMREADY,                   // A unit to tell the base it is ready
  CMD_HVAC,        // HVAC command
  CMD_SLEEP,      // Turn the module in deep sleep mode
  CMD_ASK,        // ASK if aything is required
 
 
 /* 
  SENSOR_HT,			// Temperature and humidity
  SENSOR_DUST,			// CO2 and Co
  SENSOR_PRESSURE,		// Pressure
  SENSOR_LIGHT,			// Light Level
  SENSOR_SOUND,			// Sound Level
  SENSOR_UV,			// UV level
  SENSOR_DETECT,		// Movement detector
  SENSOR_OPEN,			// Door/windows opening detection
  CMD_WHAT,				// Supported Payload type
  CMD_WHO,				// 
  RESP_WHO,				// 
  CMD_HVAC,				// HVAC command
  CMD_SLEEP,			// Turn the module in deep sleep mode
  CMD_ASK,				// Turn the module in deep sleep mode
  						// IR multimedia
						// IR radiator
*/

}
PACK;

typedef uint8_t vn_payload_type;    // Type of Payload see PayloadType
typedef uint8_t vn_payload_version; // Version of Payload, Not used.
typedef uint8_t vn_compatibility_version; // Version of Payload, Not used.
typedef uint16_t vn_payload_CRC;    // 16bits CRC for the payload, Not Used as Nrf24 CRC & ACK options is used during transmission


// Definition of the data structure for the different Payload to be used.
//WHOAMI
typedef struct {
  uint16_t   partnum;			// part number
  uint16_t   parttype;			// type of part
  uint16_t   revision;			// Revision number
  uint64_t   unitAdress;
}
PACK whoami_t;

//YOUARE
typedef struct {
  uint16_t   partid;			// part ID for the module
  uint64_t   baseCmdAdd;
  uint16_t   base_encode;	        // For encoding communication with the base
}
PACK youare_t;


//IAMREADY
typedef struct {
  uint16_t   partid;			// part ID for the module
  uint16_t   base_encode;	        // For encoding communication with the base
  uint16_t   unit_encode;	        // For encoding communication with the base
}
PACK iamready_t;



//SENSOR_HT
typedef struct {
  uint16_t   code_base;				// code to get identified when a command is back
  uint16_t   partnum;				// part num of the device
  float   humidity;       // Percentage of humidity
  float	  temperature;	// Temperature in degree centigrade	
  uint8_t   status;
}
PACK sensor_ht_t;

//SENSOR_DUST
typedef struct {
  uint16_t   LowPpulseOccupancyDUST; 	// Low Pulse Occupancy value from Dust Sensor
  float      RatioDUST;  			    // Ratio Value from Dust Sensor
  float      ConcentrationDUST;    		// PPM Concentration Value from Dust Sensor
  uint8_t    status;
}
PACK sensor_dust_t;

//SENSOR_PRESSURE
typedef struct {
  float      Pressure;			// Pressure Value
  uint8_t    status;
}
PACK sensor_pressure_t;

// SENSOR_LIGHT
typedef struct {
  uint16_t  AnalogValue;		// Raw ANALOG Value, no interpretation - it is not Lux Value
  uint8_t   status;
}
PACK sensor_light_t;

// SENSOR_SOUND
typedef struct {
  uint16_t  AnalogValue;		// Raw ANALOG Value, no interpretation
  uint16_t  MaxValue;           // Max Value within timeframe
  uint8_t   status;
}
PACK sensor_sound_t;

// SENSOR_UV
typedef struct {
  uint16_t   AnalogValue;		// Raw ANALOG Value, no interpretation
  uint8_t    IndexUV;			// UV index conversion
  uint8_t    status;
}
PACK sensor_uv_t;

// SENSOR_DETECT
typedef struct {
  uint16_t   code_base;				// code to get identified when a command is back
  uint16_t   partnum;				// part num of the device
  uint8_t   detect;				// Movement Detection
  uint8_t    status;
}
PACK sensor_detect_t;


// SENSOR_OPEN
typedef struct {
  uint8_t  open_close;			// Opening/closing
  uint8_t  windows_door;		// Windows/ External Door / Internal Door
  uint8_t  status;
}
PACK sensor_open_t;

// CMD_WHAT
typedef struct {
  uint16_t   code_base;			// code to get identified when a command is back
  uint16_t   code_sat;			// code to get identified when a command is back
  uint16_t   sensor;			// sensor supported
  uint16_t   command;			// command supported
  uint8_t    status;
}
PACK cmd_what_t;

// CMD_WHO
typedef struct {
  uint16_t   code_base;				// code to get identified when a command is back
  uint16_t   code_sat;				// code to get identified when a command is back
  uint8_t    status;
}
PACK cmd_who_t;

// RESP_WHO
typedef struct {
  uint16_t   code;				// code to get identified when a command is back
  uint16_t   partnum;			// part number
  uint16_t   parttype;			// type of part
  uint16_t   revision;			// Revision number
  uint8_t    status;
}
PACK cmd_rwho_t;

// CMD_HVAC
typedef struct {
  uint16_t   code;				// code to get identified when a command is back
  uint8_t   temperature;		// Temperature in degree centigrade
  uint8_t	fan;				// fan setup
  uint8_t	swing;				// swing setup
  uint8_t	mode;				// mode
  bool		power_switch;		// Switch power or not
  bool		is_on;				// to know whether the HVAC is ON or OFF
  uint8_t    status;
}
PACK cmd_hvac_t;

// CMD_SLEEP
typedef struct {
  uint16_t   code_base;				// code to get identified when a command is back
  uint16_t   code_sat;				// code to get identified when a command is back
  uint16_t   partnum;			// part number
  uint8_t    status;
}
PACK cmd_sleep_t;


// CMD_ASK
typedef struct {
  uint16_t   partnum;			// part number
  uint16_t   code;				// code to get identified when a command is back
  uint16_t   address;			// Adress to be used for the data exchange
  uint8_t    status;
}
PACK cmd_ask_t;


/* Not implemented Yet 
  CMD_PILOT,			// Radiator command through pilot wire
  CMD_IR,				// Multimedia IR command
  CMD_433,				// 433MHz command
  CMD_LED,				// LED color command
*/

 union COM_24g_data{
	whoami_t			WHOAMI;
	youare_t			YOUARE;
	iamready_t		IAMREADY;
  cmd_hvac_t    CMD_HVAC;
  cmd_sleep_t   CMD_SLEEP;
  cmd_ask_t     CMD_ASK;
 
/*	sensor_ht_t			SENSOR_HT;
	sensor_dust_t		SENSOR_DUST;
	sensor_pressure_t	SENSOR_PRESSURE;
	sensor_light_t		SENSOR_LIGHT;
	sensor_sound_t		SENSOR_SOUND;
	sensor_uv_t			SENSOR_UV;
	sensor_detect_t		SENSOR_DETECT;
	sensor_open_t		SENSOR_OPEN;
	cmd_what_t			CMD_WHAT;
	cmd_who_t			CMD_WHO;
	cmd_hvac_t			CMD_HVAC;*/
  } PACK;


// Payload Structure
struct Payload {
  vn_payload_type      type;
  vn_payload_version   version;
  union COM_24g_data data;
#ifdef CRC_PAYLOAD
  vn_payload_CRC payloadCRC;
#endif
}
PACK;





class COM_24g
{
  public:
  //interface element for connexion
  int _miso	;		// uno d12 (pin 18) or mega d50 (pin 22)or nano 
  int _mosi	;		// uno d11 (pin 17) or mega d51 (pin 21)or nano
  int _sck	;		// uno d13 (pin 19) or mega d52 (pin 20)or nano
  uint8_t _csn	;		// uno d10 (pin 16) or mega d48 (pin 36)or nano
  uint8_t _ce	;		// uno d9  (pin 15) or mega d49 (pin 35)or nano
  
  //PipeTable
  uint64_t _readingPipe[4];  // Table Pipe Adress to be listen (like 0xF0F0F0F0E1LL)
  uint64_t _writingPipe;  // Pipe Adress to write (like 0xF0F0F0F0E1LL)
   
  //Communication object from the RF24 library
  //RF24 		_radioCom; // Pass as a variable in the rest of the livrary because of init issue when class object is created
  uint8_t  	_channel;
  uint8_t   _maxRetry; //number of retry in case of a failure in transmission --> max is 15
  
  //Payload to send or received
  Payload 		_payload;
  vn_payload_CRC 	_crc;
  vn_payload_CRC 	_key;

 
  // Data treceived or to be sent
  PayloadType 		_dataType;
  vn_payload_version	_dataVersion;
  vn_compatibility_version  _libraryVersion;  
  union COM_24g_data 	_data;
  int  	        _unitID;


   		COM_24g(RF24 radioCom);    	                        // Object creation
  bool 		initiate(RF24 _radioCom);				// Setup the communication
  bool 		sendFrame(RF24 _radioCom);				// Send a frame through the 2.4GHz
  uint8_t  	isAvailable(RF24 _radioCom);				// Return one pipe where a data is available for reading 
  bool 		receiveFrame(RF24 _radioCom);	                        // Decode the available frame on the pipe
  void 		listeningPipe(RF24 _radioCom);			        // Listen all available pipes
 
  void          statusReportOnPC(RF24 _radioCom);                       // Use in debug in order to make sure everything is set correctly.

};

