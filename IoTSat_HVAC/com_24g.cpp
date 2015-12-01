/*
 * com_24g.cpp
 * Version 0.1 December, 2015
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

 
#include <arduino.h> 
#include <SPI.h>
#include "RF24.h"
#include "com_24g.h"

COM_24g::COM_24g(RF24 _radioCom)
{
  _miso	=50;		// uno d12 (pin 18) or mega d50 or nano 12
  _mosi	=51;		// uno d11 (pin 17) or mega d51 or nano 11
  _sck	=52;		// uno d13 (pin 19) or mega d52 or nano 13
  _csn	=48;		// uno d10 (pin 16) or mega d48 or nano 10
  _ce	=49;		// uno d9  (pin 15) or mega d49 or nano 9
}

bool COM_24g::initiate(RF24 _radioCom)
{
    bool status = false;
    int i;
	//RF24 _radioCom(_ce,_csn);
	_radioCom.begin();
	// optionally, increase the delay between retries & # of retries
	_radioCom.setRetries(15,_maxRetry);
	
	_radioCom.setChannel(_channel);

	// optionally, reduce the payload size.  seems to improve reliability
	//_radioCom.setPayloadSize(8);

	// Open pipes to other nodes for communication
	//seen if this is an issue _radioCom.openWritingPipe(_writingPipe);
	for (i=1;i<5;i++) {
		if (_readingPipe[i] != NULL) {
		_radioCom.openReadingPipe(i,_readingPipe[i]);
        printf("Now listening pipe %d at adress %x \n",i,_readingPipe[i]);

		status = true;
		}
	}
	if (status == true) {
        printf("INITIALIZATION : We are listening \n");

	_radioCom.startListening();
        _radioCom.powerUp();

	} else {
          printf("We are doing a fucking power down");

	_radioCom.powerDown();
	}
return status;
}


 
bool COM_24g::sendFrame(RF24 _radioCom)
{
	// First, stop listening so we can talk
	_radioCom.stopListening(); 
 
	//Opening the writting Pipe
	_radioCom.openWritingPipe(_writingPipe);
	delay(20);

	//put in place the Paypload
	_payload.type    = _dataType;
	_payload.version = _dataVersion;
	_payload.data    = _data;
	bool Writestatus = _radioCom.write(&_payload, sizeof(_payload));

  if (Writestatus)
      printf("Communication has been sent successfully. \n");
    else
      printf("Communication has failed.\n\r");

	//back in read mode
	_radioCom.startListening(); 
	
 
}

  
uint8_t COM_24g::isAvailable(RF24 _radioCom)
{
  uint8_t i;
  uint8_t pipeAvailable;
	if (_radioCom.available()) {
		for (i=1;i<5;i++) {
			if (_radioCom.available(&i)) {
			pipeAvailable = i;
			break;
			}
		}
	return pipeAvailable;	
	} else {
	return NULL;
	}


}


void COM_24g::listeningPipe(RF24 _radioCom)	//Listen all available pipes
{
  int i;
	for (i=1;i<5;i++) {
		if (_readingPipe[i] != NULL) {
		_radioCom.openReadingPipe(i,_readingPipe[i]);
		}
	}
  _radioCom.startListening();
  _radioCom.powerUp();

}

bool COM_24g::receiveFrame(RF24 _radioCom)	// Decode the available frame on the pipe
{

int   len 	= _radioCom.getDynamicPayloadSize();  	// Size of the payload to read
bool  done 	= _radioCom.read(&_payload, len);        // Read the payload

if(done){
//_dataType        = _payload.type;
_dataVersion     = _payload.version;

  switch (_payload.type ) {
    /*case CMD_WHO:
    _data.CMD_WHO.partnum   = _payload.data.CMD_WHO.partnum;
    _data.CMD_WHO.revision  = _payload.data.CMD_WHO.revision;
    _data.CMD_WHO.parttype  = _payload.data.CMD_WHO.parttype;
    return true;
    break;
    */
    case YOUARE:
    _data.YOUARE.partid        = _payload.data.YOUARE.partid;
    _data.YOUARE.baseCmdAdd    = _payload.data.YOUARE.baseCmdAdd;
    _data.YOUARE.base_encode   = _payload.data.YOUARE.base_encode;
    _dataType                  = YOUARE;
    return true;
    break;
   
    
    default:
        //serial.println("Unknown message.");
    break;
    
  }

} else {
 //serial.println("Error of read on the Paypload"); 
 return false;
}


}

void COM_24g::statusReportOnPC(RF24 _radioCom)	// Decode the available frame on the pipe
{
_radioCom.printDetails();

}






