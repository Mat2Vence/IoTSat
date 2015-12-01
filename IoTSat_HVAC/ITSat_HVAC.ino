/* ***************** FULL CUSTOM IoT Home Automation *****************
 * 
 * homeautomation.emtee.fr                                   Dec, 2015
 * 
 * *******************************************************************
 * IoTSat_HVAC.ino
 * Version 0.1 December, 2015
 * Copyright 2015 Mathieu THOMAS
 * For details, see http://www.emtee.fr
 *
 *
 * 
 * IoT Satellite using custom com-24g library
 * 
 * Need to work in parallel with the following modules :
 *  - IoT Bridge (Arduino Bridge between 2.4GHz and Ethernet)
 *  - IoT Home Server (Lamp server that manage the global home automation)
 *
 *
 */
 
#include <stdio.h>
#include <SPI.h>
#include "com_24g.h"
#include "RF24.h"
#include "printf.h"
//#include <Printf.h>



// *************************************************************************************************************************
// *************************************************************************************************************************
// *************************************************************************************************************************
//
// **********************************************     VARIABLE SETUP     ***************************************************        
//
// *************************************************************************************************************************
// *************************************************************************************************************************
// 


//Instantiation of the COM object.
// conf for the mega 
//uint8_t csn= 48;
//uint8_t ce= 49;

uint8_t pipeNo;

//Configuration for the Nano
uint8_t csn= 10;
uint8_t ce= 9;

RF24 _myRF24(ce,csn);
COM_24g myCom(_myRF24);




// *************************************************************************************************************************
// *************************************************************************************************************************
// *************************************************************************************************************************
//
// **********************************************     SIDE FUNCTIONS     ***************************************************        
//
// *************************************************************************************************************************
// *************************************************************************************************************************
// 



/********************************************************************************
* long readVcc() 
*  - return the internal Vcc value in mV
*/
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

/********************************************************************************
* void power_on (int powerPin )
*  - set High level to powerPin
*/
void power_on (int powerPin) {
  pinMode(powerPin,OUTPUT);
  digitalWrite(powerPin,HIGH);
  delay(200); // delay for power sloop
}

/********************************************************************************
* void power_off (int powerPin)
*  - set Low level to powerPin
*/
void power_off (int powerPin) {
  pinMode(powerPin,OUTPUT);
  digitalWrite(powerPin,LOW);
}


/*********************************************************************************
* void print_dht()
*  - print dht sensor values
*  - send Payload values
*/
void print_dht() { 
  delay(200); // let time to warm up the DHT
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
  Serial.print("DHT:\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");  
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");
  // Prepare & Send the Payload
  payload.type = SENSOR_DHT;
  payload.data.SENSOR_DHT.humidity = h;
  payload.data.SENSOR_DHT.temperature = t;
  payload.data.SENSOR_DHT.dewpoint = hi;
  payload.data.SENSOR_DHT.status = 0;
  doSendMsg();  
}


/*********************************************************************************
* void mywatchdogenable()
*  - set watch dog & sleep function
*/
void mywatchdogenable() 
{ 
  MCUSR = 0; 
  WDTCSR = _BV (WDCE) | _BV (WDE); 
  WDTCSR = _BV (WDIE) | _BV (WDP3) | _BV (WDP0); //délai de 8 secondes 
  wdt_reset(); 
  //ADCSRA = 0; //désactive ADC 
  set_sleep_mode (SLEEP_MODE_PWR_DOWN); 
  sleep_enable(); 
  MCUCR = _BV (BODS) | _BV (BODSE); 
  MCUCR = _BV (BODS); 
  sleep_cpu(); 
  sleep_disable(); 
}



// *************************************************************************************************************************
// *************************************************************************************************************************
// *************************************************************************************************************************
//
// **********************************************     SETUP     ************************************************************        
//
// *************************************************************************************************************************
// *************************************************************************************************************************
// *************************************************************************************************************************

void setup(void)
{

 Serial.begin(9600);
printf_begin();
  Serial.println("Preparing NRF24L01 interface");
    printf("\n check if the printf function is working\n");

myCom._miso	=12;		// uno d12 (pin 18) or mega d50 or nano 12
myCom._mosi	=11;		// uno d11 (pin 17) or mega d51 or nano 11
myCom._sck	=13;		// uno d13 (pin 19) or mega d52 or nano 13
myCom._csn	=10;		// uno d10 (pin 16) or mega d48 or nano 10
myCom._ce	  =9;		// uno d9  (pin 15) or mega d49 or nano 9

//Setting of the myCom object
  myCom._readingPipe[1] = 0xFF00000002LL;
 
  myCom._writingPipe    = 0xFF00000001LL; 
  //myCom._writingPipe    = 0x0000000001LL; 
 
  myCom._channel        =3;
  myCom._maxRetry       =5; //number of retry in case of a failure in transmission --> max is 15

 myCom.initiate(_myRF24);
   Serial.println("Start COM");
    myCom.statusReportOnPC(_myRF24);


// Init of the Sat
// STEP 1 : WHOAMI    
    myCom._dataType    = WHOAMI;
    myCom._dataVersion  = 1;
    myCom._data.WHOAMI.partnum=0x001A;
    myCom._data.WHOAMI.parttype=0x0A0B;
    myCom._data.WHOAMI.revision=0x0001;
    
    Serial.println("Send WHOAMI");
    myCom.sendFrame(_myRF24);
    Serial.println("Done");

// STEP 2 : Listen for the Base answer
    Serial.println("Wait for the answer");
    myCom.listeningPipe(_myRF24);
    while (_myRF24.available(&pipeNo)) {
         Serial.println("There is a frame available");
         bool done = false;
         int len;
         while (!done) {
               done=myCom.receiveFrame(_myRF24);
               Serial.println("data read");
               Serial.println(myCom._data.YOUARE.partid);    
               //Serial.println(myCom._data.YOUARE.baseCmdAdd);    
               //Serial.println(myCom._data.YOUARE.base_encode);    

         }
    }

// STEP 3 : Write required Info in EEEPROM and Send READY


}



// *************************************************************************************************************************
// *************************************************************************************************************************
// *************************************************************************************************************************
//
// **********************************************     MAIN PROGRAM     *****************************************************        
//
// *************************************************************************************************************************
// *************************************************************************************************************************
// *************************************************************************************************************************



void loop(void)
{
// /////////////////////////////////////////////////////////////
//STEP 1 : WAKE-UP



// /////////////////////////////////////////////////////////////
//STEP 2 : Send the command CMD_ASK 




// /////////////////////////////////////////////////////////////
//STEP 3 : Start Communication with IoT Server
 
myCom.listeningPipe(_myRF24);
while (_myRF24.available(&pipeNo)) {
        Serial.println("There is a frame available");
        bool done = false;
        int len;
          while (!done) {
            done=myCom.receiveFrame(_myRF24);
            Serial.println("data read");
            
            // Check what kid of payload must be process
            switch (myCom._payload) {
              case YOUARE:    // IoT Server is transmitting the ID number of the device
                //Store the ID within the EEEPROM

               //Store the ID within a standard Variable


               //Send the answer IAMREADY

              break;
   

              case CMD_SLEEP :    // IoT Server ask for a switch of of the satellite
                // Go to sleep


                break;
                
              case CMD_HVAC : // IoT Server is sending a HVAC command
                // Sending the HVAC command

                //Sending back the command received for cross check and log


                break;

            } // end of switch
          } // end of while receviving
      } // end of while listening
 



// /////////////////////////////////////////////////////////////
//STEP 4 : Go back to sleep if not already done.
 



     
  
}
