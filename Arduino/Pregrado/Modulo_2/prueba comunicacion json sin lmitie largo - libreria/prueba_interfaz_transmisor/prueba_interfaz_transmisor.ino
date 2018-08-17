
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Interface.h>
/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
//bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/


const uint64_t addresses[2] = {0xF0F0F0F0E1,0xF0F0F0F0D2};
bool moduloValido[2] = {false,false};
// pin del relevo
byte rel_pin = 5;
uint8_t buffer_datos_recividos = 500;
bool depuracion =  true;

unsigned long tiempo_anterior = 0;
bool on_off = true;
int contador = 0;

void setup() {
  Serial.begin(115200);
  configurar_radio_transmisor();
  // configuracion puerto relevo
  pinMode(rel_pin,OUTPUT);
  digitalWrite(rel_pin,LOW);
}

void loop() {
  if(millis() > (tiempo_anterior+500) && moduloValido[0]){
    StaticJsonBuffer<200> jsonBuffer;
    /****************** JSON Config ***************************/
    JsonObject& root = jsonBuffer.createObject();
    root["Sensor"] = "relevo";
    root["Tiempo"] = 0;
    if(on_off){
      root["Funcion"] = "Encender";on_off=false;
    }else{
      root["Funcion"] = "Apagar";on_off=true;
    }
    
    String string;
    root.printTo(string);
    Serial.println();
    Serial.println(string);
    // enviamos datos
    enviarMensaje(string);

    tiempo_anterior = millis(); // actualizamos el tiempo anterior
  }
  if(!moduloValido[0]){identificarModulos();}
} // Loop

///// envio paquetes si hay datos en el serial
//void pruebaEnvioSerial(){
//  if (Serial.available()){
//    /****************** JSON Config ***************************/
//    String str = String(char(Serial.read()));
//    Serial.println("Enviando:... "+str);
//    if(!enviarMensaje(str)){
//      Serial.println(F("failed"));
//    }else{
//      Serial.println(F("Enviado"));
//    }
//    Serial.read();
//  } // serial av
//}

//for nrf24 debug
int serial_putc( char c, FILE * ){
  Serial.write( c );
  return c;
} 

//for nrf24 debug
void printf_begin(void){
  fdevopen( &serial_putc, 0 );
}
