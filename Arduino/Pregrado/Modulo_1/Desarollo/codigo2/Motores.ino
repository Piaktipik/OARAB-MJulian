
// ************************************** Motores ************************************** 
// -------------------- puertos usados del arduino
// motor derecho - m1
const byte pin_dire_moto_dere = 13;  
const byte pin_velo_moto_dere = 12;
// motor izquierdo - m2
const byte pin_dire_moto_izqu = 10;
const byte pin_velo_moto_izqu = 11;

// -------------------- variables de velocidad usadas en las funciones de movimiento
byte velo_moto_izqu = 120; // izquierdo
byte velo_moto_dere = 120; // derecho

//byte velo_esta = 70;      // velocidad entandar de el desplazamiento
//byte valo_ajus_velo = 5;  // valor de ajuste -> se le suma al motor derecho

// variables control
unsigned long tiempoUltimoMovimiento = 0; // variable auxiliar para tener un registro del tiempo desde que se modifico el movimiento por ultima vez
unsigned int tiempoMinimoDuracionMovimientoActual = 1000;  // tiempo en milisegundos que desea moverse una determinada direccion
// variables para el manejo del estado del movimiento
#define Detenido 0
#define Adelante 11
#define Atras 12
#define Derecha 13
#define DerechaSuabe 14
#define Izquierda 15
#define IzquierdaSuabe 16
byte estadoMovimientoActual = Detenido;
//byte estadoMovimientoAnterior = Detenido;

// convierte un char a dato para la funcion de movimiento
void mover(char dato){
  byte estado = Detenido;
  // modificamos el movimiento
  switch(dato){
    case 'p'        : estado = Detenido;
    break;case 'a'  : estado = Adelante;
    break;case 'h'  : estado = Atras;
    break;case 'd'  : estado = Derecha;
    break;case 'e'  : estado = DerechaSuabe;
    break;case 'i'  : estado = Izquierda;
    break;case 'j'  : estado = IzquierdaSuabe;
    break;
  }
  movimiento(estado);
}

// ejecuta un movimineto del robot
void movimiento(byte movimientoDeseado){
  // si el movimineto solicitado es diferente se realiza
  if(movimientoDeseado != estadoMovimientoActual){
    
    // modificamos el movimiento
    switch(movimientoDeseado){
      case Detenido             : Detener();
      break;case Adelante       : adelante();
      break;case Atras          : atras();
      break;case Derecha        : derecha();
      break;case DerechaSuabe   : derechaSuabe();
      break;case Izquierda      : izquierda();
      break;case IzquierdaSuabe : izquierdaSuabe();
      break;
    }
    // si estoy en depuracion y el estado de movimiento cambio, informo el nuevo estado
    if(depuracion)InformarEstadoMovimiento();
  }
  tiempoUltimoMovimiento = millis();
}

void moverDurante(unsigned int tiempoDeseado){
  tiempoMinimoDuracionMovimientoActual = tiempoDeseado;
  // requiere que la funcion actulizarMovimiento sea llamada para detener el movimiento actual
}

void moverHaciaDurante(byte dato,unsigned int tiempoDeseado){
  movimiento(dato);
  tiempoMinimoDuracionMovimientoActual = tiempoDeseado;
  // requiere que la funcion actulizarMovimiento sea llamada para detener el movimiento actual
}

void actualizarMovimiento(){
  // detiene el movimiento pasado el tiempoMinimoDuracionMovimientoActual
  if(millis() > tiempoUltimoMovimiento + tiempoMinimoDuracionMovimientoActual && (tiempoMinimoDuracionMovimientoActual!= 0)){
    movimiento(Detenido);
  }
}

void modificarVelocidadI(byte nuevaVelocidadIzquierda){
  if(velo_moto_izqu != nuevaVelocidadIzquierda){
    velo_moto_izqu = nuevaVelocidadIzquierda;
    // si el motor esta siendo usado, modificamos de imediato su velocidad
    if(estadoMovimientoActual != Detenido && estadoMovimientoActual != IzquierdaSuabe){
      analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
    }
  }
}

void modificarVelocidadD(byte nuevaVelocidadDerecha){
  if(velo_moto_dere != nuevaVelocidadDerecha){
    velo_moto_dere = nuevaVelocidadDerecha;
    // si el motor esta siendo usado, modificamos de imediato su velocidad
    if(estadoMovimientoActual != Detenido && estadoMovimientoActual != DerechaSuabe){
      analogWrite(pin_velo_moto_dere, velo_moto_dere);
    }
  }
}

void InformarEstadoMovimiento(){
  // Informamos el estado del movimiento
  switch(estadoMovimientoActual){
    case Detenido             : Serial.println("Estado Movimiento: Detenido");
    break;case Adelante       : Serial.println("Estado Movimiento: Adelante");
    break;case Atras          : Serial.println("Estado Movimiento: Atras");
    break;case Derecha        : Serial.println("Estado Movimiento: Derecha");
    break;case DerechaSuabe   : Serial.println("Estado Movimiento: Derecha Suabe");
    break;case Izquierda      : Serial.println("Estado Movimiento: Izquierda");
    break;case IzquierdaSuabe : Serial.println("Estado Movimiento: Izquierda Suabe");
    break;
  }
}

// --------------------- activacion de los motores --------------------
void activarMotores(){
  // ----------------- motores ----------------- 
  // se declaran como salidas
  pinMode(pin_dire_moto_dere,OUTPUT);
  pinMode(pin_velo_moto_dere,OUTPUT);
  pinMode(pin_dire_moto_izqu,OUTPUT); 
  pinMode(pin_velo_moto_izqu,OUTPUT);

  Detener(); // inicializo el robot quieto
}

// --------------------- movimientos individuales --------------------
void Detener(){
  estadoMovimientoActual = Detenido;
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, 0);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, 0);
}
void adelante(){
  estadoMovimientoActual = Adelante;
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void atras(){
  estadoMovimientoActual = Atras;
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void derecha(){
  estadoMovimientoActual = Derecha;
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor -izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor -derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void derechaSuabe(){
  estadoMovimientoActual = DerechaSuabe;
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor -izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor -derecho
  analogWrite(pin_velo_moto_dere, 0);
}
void izquierda(){
  estadoMovimientoActual = Izquierda;
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor -izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor -derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}  
void izquierdaSuabe(){
  estadoMovimientoActual = IzquierdaSuabe;
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor -izquierdo
  analogWrite(pin_velo_moto_izqu, 0);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor -derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}  



