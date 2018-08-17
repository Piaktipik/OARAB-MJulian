
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
//2Node
byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);

  radio.stopListening();                                    // First, stop listening so we can talk.
    
}

void loop() {
  
  if ( Serial.available()){
    
    char c = Serial.read();
    if ( c == '1'){      
      Serial.println(F("Enviando 1..."));
      
     if (!radio.write( &c, sizeof(char) )){
       Serial.println(F("failed"));
     }
     Serial.println(F("Enviado"));
   }else
    if ( c == '0'){
      Serial.println(F("Enviando 0..."));
      if (!radio.write( &c, sizeof(char) )){
       Serial.println(F("failed"));
      }
      Serial.println(F("Enviado"));  
    }
  } // serial av

} // Loop

