
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

//#include <Interface.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
//bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

const uint64_t addresses[2] = {0xF0F0F0F0E1,0xF0F0F0F0D2};

// pin del relevo
byte rel_pin = 5;
uint8_t buffer_datos_recividos = 500;
bool depuracion =  true;

void setup() {
  Serial.begin(115200);
  configurar_radio_receptor();
  // configuracion puerto relevo
  pinMode(rel_pin,OUTPUT);
  digitalWrite(rel_pin,LOW);
}

void loop() {
  StaticJsonBuffer<100> jsonBuffer;
  
  String datos = leer_datos();
  if(false & depuracion && datos != ""){
    Serial.print(" ");
    Serial.println(datos);
  }
  if(datos != ""){
    JsonObject& root = jsonBuffer.parseObject(datos);
    // Test if parsing succeeds.
    if (!root.success()) {
      Serial.println(" parseObject() failed");
      return;
    } else{
      Serial.println(" parseObject() ok ");
    }
    
    String sensor = root["Sensor"];
    long tiempo = root["Timepo"];
    String funcion = root["Funcion"];
  
    // Print values.
    Serial.print(sensor);
    Serial.print(tiempo);
    Serial.println(funcion);

    if (funcion == "Conectar")datos="1";
    else if (funcion == "Desconectar")datos="0";
  }
  
  // ejecutamos la accion requerida
  if (datos == "1"){
    conectar();
  }else if (datos == "0"){
    desconectar();
  }
  
  // prueba por serial del relevo
  if (Serial.available()){
    char c = Serial.read();
    if ( c == '1'){
      digitalWrite(rel_pin,HIGH);
    }else if ( c == '0'){
      digitalWrite(rel_pin,LOW);
    }
  } // serial av
  //delay(100);
  //Serial.println(".");
} // Loop

//for nrf24 debug
int serial_putc( char c, FILE * ){
  Serial.write( c );
  return c;
} 

//for nrf24 debug
void printf_begin(void){
  fdevopen( &serial_putc, 0 );
}
