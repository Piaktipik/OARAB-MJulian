
// ************************************** inicio ************************************** 
// por : Julian Galvez Serna - 2016
// ******************************* seleccion de programa ******************************
#define Encoders                1
#define SensoresSuelo           2
#define SensoresDistancia       3
#define SensoresDistanciaRadar  4
#define ControlSerialMotores    5
#define EvasorObstaculos        6
#define SeguidorLinea           7
#define SeguidorObjetos         8
#define Nada                    9

// Variables depuracion
byte ver = Nada;
bool depuracion =  true;    // muestra por el puerto serial el registro de las actividades y las mediciones de los sensores

#include <Interface.h>
Interface *interface = new Interface();
// ************************************** Funciones Principales ************************************** 
void setup() {Setup();} // configraciones iniciales

void loop() {Loop();}   // ciclo infinito, "programa principal"



