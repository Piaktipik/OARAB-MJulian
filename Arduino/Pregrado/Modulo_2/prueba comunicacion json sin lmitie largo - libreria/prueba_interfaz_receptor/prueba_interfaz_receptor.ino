// por: Julian Galvez Serna

#include <Interface.h>
Interface *interface = new Interface();

bool depuracion =  true;
const byte rel_pin = 5; // pin fisico del relevo
bool relevoOn = true; // solo proposito de pruebas

/***********************************************************************/
void setup() {
  Serial.begin(115200);
  if(depuracion)Serial.println("Inicicando...");
  
  activarRelevo();
  
  (*interface).activarDepuracion(Serial);
  (*interface).usarNRF24L01();
  
  // ************** se declaran las funciones de este modulo ***************
  
  (*interface).agregarObjeto("Lampara");
  (*interface).agregarFuncion("Encender",&conectar);       
  (*interface).agregarFuncion("Apagar",&desconectar);
  
  if(depuracion)Serial.println("Modulo Iniciado.");
}

/***********************************************************************/
void loop() {
  (*interface).revisarInterface();
  pruebaSerialRelevo();
}

/***********************************************************************/
inline void desconectar(){
  Serial.println("Relevo Desconectado");
  if(relevoOn)digitalWrite(rel_pin,HIGH);
}

inline void conectar(){
  Serial.println("Relevo Conectado");
  if(relevoOn)digitalWrite(rel_pin,LOW);
}

// configuracion puerto relevo
inline void activarRelevo(){
  pinMode(rel_pin,OUTPUT);
  digitalWrite(rel_pin,LOW);
}

inline void pruebaSerialRelevo(){
  // prueba por serial del relevo
  if (Serial.available()){
    char c = Serial.read();
    if ( c == '1'){
      digitalWrite(rel_pin,HIGH);
    }else if ( c == '0'){
      digitalWrite(rel_pin,LOW);
    }
  }
}



