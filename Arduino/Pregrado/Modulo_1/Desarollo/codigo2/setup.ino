// libreria usada pra manejar la medicion y aculizacion de los sensores
#include <DueTimer.h>

void Setup() {
  // puerto serial depuracion:
  Serial.begin(250000);
  if(depuracion)Serial.println("Inicicando...");
  
  // ----------------- Raspberry ----------------- 
  Serial1.begin(115200); // puerto de conexion con la raspberry
  
  (*interface).activarDepuracion(Serial);
  (*interface).usarSerial(Serial1);
  

  // ************** se declaran las funciones de este modulo ***************
  
  (*interface).agregarObjeto("Robot");
  (*interface).agregarFuncion("Evadir Obstaculos",&evadirObstaculos);       
  (*interface).agregarFuncion("Siguir Objeto",&seguidorObjetos);
  (*interface).agregarFuncion("Detener",&RobotDetenido); 
  (*interface).agregarFuncion("Mover Adelante",&RobotAdelante);  
  (*interface).agregarFuncion("Mover Atras",&RobotAtras);  
  (*interface).agregarFuncion("Mover Izquierda",&RobotIzquierda);  
  (*interface).agregarFuncion("Mover Derecha",&RobotDerecha);  
  
  (*interface).agregarObjeto("Ultrasonido Izquierdo");
  (*interface).agregarVariableEstado("Ultrasonido Izquierdo",&ultrasonidoConectadoI);
  (*interface).agregarObjeto("Ultrasonido Frontal","Robot");
  (*interface).agregarVariableEstado("Ultrasonido Frontal",&ultrasonidoConectadoF);
  (*interface).agregarObjeto("Ultrasonido Derecho","Robot");
  (*interface).agregarVariableEstado("Ultrasonido Derecho",&ultrasonidoConectadoD);

  (*interface).agregarObjeto("Sensor Linea Izquierda","Robot");
  (*interface).agregarVariableEstado("Sensor Linea Izquierda",&sensorSueloConectadoI);
  (*interface).agregarObjeto("Sensor Linea Derecho","Robot");
  (*interface).agregarVariableEstado("Sensor Linea Derecho",&sensorSueloConectadoD);


  /*
  {"Robot":
  [{"Funciones":["Adelante Evadiendo Obstaculos","Adelante Siguiendo Objeto","Detener","Mover Adelante","Mover Atras","Mover Izquierda","Mover Derecha"]},
  {"Ultrasonido Izquierdo":[{"Estados":["Estado Ultrasonido Izquierdo"]}]},
  {"Ultrasonido Frontal":[{"Estados":["Estado Ultrasonido Frontal"]}]},
  {"Ultrasonido Derecho":[{"Estados":["Estado Ultrasonido Derecho"]}]},
  {"Sensor Linea Izquierda":[{"Estados":["Estado Sensor Linea Izquierda"]}]},
  {"Sensor Linea Derecho":[{"Estados":["Estado Sensor Linea Derecho"]}]}]}
  
  
  */
  // {"Robot":"Adelante Evadiendo Obstaculos"}
  // {"Robot":"Detener"}  HOLA-IDENTIFICATE {"Robot":"Mover Adelante"}
  // ----------------- se activan los modulos ----------------- 
  activarSesoresColisiones(); // configuramos los puertos y activamos interrupciones para detectar las colisiones
  activarSensoresDistancia();
  activarSensoresSuelo();
  activarEncoder1(pinA1,pinB1);
  activarEncoder2(pinA2,pinB2);
  activarMotores();
  
  // configuracion del temporizador para la actualizacion de los sensores
  Timer.getAvailable().attachInterrupt(actulizar).setFrequency(100).start(); // llamamos n veces por segundo 

  if(depuracion){Serial.println("Modulo Iniciado.");}
}// fin setup
