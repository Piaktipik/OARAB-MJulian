
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
//#define

byte ver = ControlSerialMotores;

bool depuracion =  true;    // muestra por el puerto serial el registro de las actividades y las mediciones de los sensores
byte pinRaspberryHabla = 2; // pin usado para verificar la comunicacion con la raspberry por puerto serial


//#include <Interface.h>
//Interface interface;
// ************************************** Funciones Principales ************************************** 
void setup() {Setup();} // configraciones iniciales

void loop() {Loop();}   // ciclo infinito, "programa principal"



