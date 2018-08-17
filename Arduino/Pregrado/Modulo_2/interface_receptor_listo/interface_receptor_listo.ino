// por: Julian Galvez Serna

#include <Interface.h>
Interface *interface = new Interface();

const byte rel_pin = 5; // pin fisico del relevo

/***********************************************************************/
void setup() {
  Serial.begin(115200);
  Serial.println("Inicicando...");
  
  activarRelevo();
  
  (*interface).activarDepuracion(Serial);
  (*interface).usarNRF24L01();
  
  // ************** se declaran las funciones de este modulo ***************
  
  (*interface).agregarObjeto("Lampara");
  (*interface).agregarFuncion("Encender",&conectar);       
  (*interface).agregarFuncion("Apagar",&desconectar);
  
  // fin de la declaracion del modulo a la interface
  Serial.println("Modulo Iniciado.");
}

/***********************************************************************/
void loop() {
  
  (*interface).revisarInterface();
}

/***********************************************************************/
inline void desconectar(){
  Serial.println("Relevo Desconectado");
  digitalWrite(rel_pin,HIGH);
}

inline void conectar(){
  Serial.println("Relevo Conectado");
  digitalWrite(rel_pin,LOW);
}

// configuracion puerto relevo
inline void activarRelevo(){
  pinMode(rel_pin,OUTPUT);
  digitalWrite(rel_pin,LOW);
}


