
#include <Interface.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include "RF24.h"

/****************** JSON Config ***************************/

/**********************************************************/


/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
// 1Node
byte addresses[][6] = {"1Node","2Node"};

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
  //JsonObject& root = jsonBuffer.createObject();
  String datos = leer_datos();
  if(false && depuracion && datos != ""){
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
    
    String sensor = root["s"];
    long tiempo = root["t"];
    String funcion = root["F"];
  
    // Print values.
    Serial.print(sensor);
    Serial.print(tiempo);
    Serial.println(funcion);

    if (funcion == "Cone")datos="1";
    else if (funcion == "Desc")datos="0";
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
} // Loop
