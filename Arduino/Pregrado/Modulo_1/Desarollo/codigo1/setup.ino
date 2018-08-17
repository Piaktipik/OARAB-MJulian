// libreria usada pra manejar la medicion y aculizacion de los sensores
#include <DueTimer.h>

void Setup() {
  // puerto serial depuracion:
  Serial.begin(115200);
  
  // ----------------- Raspberry ----------------- 
  Serial1.begin(115200); // puerto de conexion con la raspberry
  pinMode(pinRaspberryHabla,INPUT);

  // ----------------- se activan los modulos ----------------- 
  activarSesoresColisiones(); // configuramos los puertos y activamos interrupciones para detectar las colisiones
  activarSensoresDistancia();
  activarSensoresSuelo();
  activarEncoder1(pinA1,pinB1);
  activarEncoder2(pinA2,pinB2);
  activarMotores();
  
  //(*interface).agregarObjeto("Lampara");
  //(*interface).agregarFuncion("Encender",&conectar);       
  //(*interface).agregarFuncion("Apagar",&desconectar);
  //(*interface).agregarVariableEstado("Estado Relevo",&relevoOn);
  
  // interface.agregarFuncion("Conectar",&conectar);    // funcion del objeto Lampara
  // interface.agregarObjeto(HIJO,"Robot");                  // objeto de la funcion lampara
  // interface.agregarFuncion("Conectar",&conectar);    // funcion del objeto Robot
  // interface.agregarObjeto(HERMANO,"Robot");                  // objeto de la funcion lampara
  // interface.agregarFuncion("Conectar",&conectar);    // funcion del objeto Robot
  
  
  // configuracion del temporizador para la actualizacion de los sensores
  Timer.getAvailable().attachInterrupt(actulizar).setFrequency(100).start(); // llamamos 100 veces por segundo 
  
}// fin setup
