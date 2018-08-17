// por: Julian Galvez Serna

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ------------------------------------------------------ motores y actuadores ------------------------------------------------------
// ----------- conexion motores -----------
// ----- motores -----
// motor 1 - m1
const byte pin_dire_moto_1 = 4;
const byte pin_velo_moto_1 = 5;

// motor 2 - m2
const byte pin_dire_moto_2 = 7;
const byte pin_velo_moto_2 = 6;

// variables que contienen la velocidad de los motores cargadas al llamar las funciones de movimiento
byte velo_moto_1 = 60;
byte velo_moto_2 = 60;

byte estadoMotores = 0;
/*
 * Estado de los motores 
 * 0 -> Parados
 * 
 */

bool obstaculo = false;

// Pines Encoder Motor 1
int pinAM1 = 18; //(pines Serial 1)
int pinBM1 = 19;

// Pines Encoder Motor 2
int pinAM2 = 20; //(pines I2C)
int pinBM2 = 21;

// Variables estado Encoder Motor 1
int caM1 = 0;
int cbM1 = 0;
int est_encM1 = 0;
long pas_totM1 = 0;

// Variables estado Encoder Motor 2
int caM2 = 0;
int cbM2 = 0;
int est_encM2 = 0;
long pas_totM2 = 0;

// Pruebas Busqueda Angulo
int set_point = 0;
int offset = 0;

// variables para imprimir solo cambios por serial
int capM1 = -1;
int capM2 = -1;

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void setup() {

  // ----------------- comunicaciones seriales -----------------
  Serial.begin(115200);       // comunicacion con el pc - pruebas

  activarEncoders(pinAM1, pinBM1, pinAM2, pinBM2);

  // ----------------- motores -----------------
  // se declaran como salidas
  pinMode(pin_dire_moto_2, OUTPUT);
  pinMode(pin_velo_moto_2, OUTPUT);
  pinMode(pin_dire_moto_1, OUTPUT);
  pinMode(pin_velo_moto_1, OUTPUT);

  parar(); // inicializo el robot quieto

}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void loop() {

  delay(100);
  //int est_encM1 = obt_est(pinAM1,pinBM1);

  // imprimimos solo los cambios
  if (capM1 != caM1 || capM2 != caM2) {
    capM1 = caM1;
    capM2 = caM2;
    /*
    Serial.print("CaM1 = " + String(caM1));
    Serial.print(" , CbM1 = " + String(cbM1));
    Serial.print(" , Est_encM1 = " + String(est_encM1));
    Serial.print(" , pas_totM1 = " + String(pas_totM1));
    Serial.print(" , ");
    Serial.print("CaM2 = " + String(caM2));
    Serial.print(" , CbM2 = " + String(cbM2));
    Serial.print(" , Est_encM2 = " + String(est_encM2));
    Serial.print(" , pas_totM2 = " + String(pas_totM2));
    Serial.println();
    */
    Serial.print(String(pas_totM1));
    Serial.print("," + String(pas_totM2));
    Serial.println();
  }


  // recivo comandos por el xbee serial
  if (Serial.available() > 0) {
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }

  // codigo seguimiento de posicion
  /*
    if(pas_totM1 > set_point+offset){velocidad((pas_totM1-set_point)*40);atras();}
    else if(pas_totM1 < set_point-offset){velocidad((pas_totM1-set_point)*40);adelante();}

    else{
    parar();
    }
  */
}  // fin loop

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ



