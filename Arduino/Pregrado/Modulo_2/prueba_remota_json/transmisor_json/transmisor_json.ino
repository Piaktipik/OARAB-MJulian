
#include <Interface.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include "RF24.h"

/****************** JSON Config ***************************/
StaticJsonBuffer<200> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
/**********************************************************/


/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
//2Node
byte addresses[][6] = {"1Node","2Node"};

//uint8_t buffer_datos_enviados = 100;
bool depuracion =  true;

unsigned long tiempo_anterior = 0;
bool on_off = true;
/*************************Setup****************************/
void setup() {
  Serial.begin(115200);
  configurar_radio_transmisor();
}
/**********************************************************/

/************************Loop******************************/
void loop() {
  if(millis() > tiempo_anterior+500){
    
    root["s"] = "rel";
    root["t"] = 0;
    if(on_off){
      root["F"] = "Cone";on_off=false;
    }else{
      root["F"] = "Desc";on_off=true;
    }
    
    String string;
    root.printTo(string);
    string = completarString(string);
    Serial.println(string);
    enviar_datos_mostrar(string);
    
    tiempo_anterior = millis(); // actualizamos el tiempo anterior
  }
  
  
  if ( Serial.available()){
    String str = String(char(Serial.read()));
    Serial.println("Enviando:... "+str);
    if(!enviar_datos(str)){
      Serial.println(F("failed"));
    }else{
      Serial.println(F("Enviado"));
    }
  } // serial av

} // Loop
/**********************************************************/

String completarString(String incompleto){
  String completo = "";
  for(int i = 0; i<incompleto.length();i++){
    if (incompleto[i] == '"'){
      completo += '\"';
    }else{
      completo += incompleto[i];
    }
  }
  return completo;
}

