/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

byte rel_pin = 5;


/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
// 1Node
byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  radio.begin();
  
  pinMode(rel_pin,OUTPUT);
  digitalWrite(rel_pin,LOW);
  
  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
/****************** Pong Back Role ***************************/
    char data;
    
    if(radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &data, sizeof(char) );                 // Get the payload
      }
     
      Serial.print("leido: ");
      Serial.println(data); 

      if (data == '1'){
        digitalWrite(rel_pin,HIGH);
      }else if ( data == '0'){
        digitalWrite(rel_pin,LOW);
      }
      
   }

   if ( Serial.available()){
    
    char c = Serial.read();
     if ( c == '1'){
        digitalWrite(rel_pin,HIGH);
     }else if ( c == '0'){
        digitalWrite(rel_pin,LOW);
     }
    } // serial av

   
} // Loop
