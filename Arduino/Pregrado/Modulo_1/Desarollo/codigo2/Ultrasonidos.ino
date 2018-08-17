
// ************************************** Sensores de Distancia - (Ultrasonido) ************************************** 
// --------------------  puertos usados del arduino 
const byte pinSensorUltrasonidoI = 50;   // pin ultrasonido - Izquierdo         
const byte pinSensorUltrasonidoF = 51;   // pin ultrasonido - Frontal
const byte pinSensorUltrasonidoD = 49;   // pin ultrasonido - Derecho

// -------------------- variables medicion
volatile unsigned long tiempoMedicionPulsoAltoI,tiempoMedicionPulsoAltoF,tiempoMedicionPulsoAltoD;
volatile unsigned long tiempoPulsoAltoI,tiempoPulsoAltoF,tiempoPulsoAltoD;
int medidaDistanciaDerecha, medidaDistanciaFrontal, medidaDistanciaIzquierda;

// ************************************** Servos direccion de medicion **************************************
#include <Servo.h> 
//  0.1s/60 grados velocidad maxima
Servo servoI;  // servo Izquierdo
Servo servoF;  // servo Frontal
Servo servoD;  // servo Derecho

// -------------------- operacion de servos
byte posicionRadarI = 0;   // memoria de posicion del servo1
byte posicionRadarF = 0;   // memoria de posicion del servo2
byte posicionRadarD = 0;   // memoria de posicion del servo3

bool direccionRadarCrecienteI = true;    // contador creciente
bool direccionRadarCrecienteF = true;
bool direccionRadarCrecienteD = true;

// -------------------- variables radar
byte angulosRadarI[] = {50,60,70,80,90,100};
//{30,40,50,60,70,80,90,100,110,120,130,140,150,160,170};
//{10,10,20,20,30,30,40,40,50,50,60,60,70,70,80,80,90,90,100,110,120,130,140,150,160,170};
//{10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,100,110,120,130,140,150,160,170};
const byte cantidadRadarI = sizeof(angulosRadarI);
int medidasRadarI[cantidadRadarI+1];

byte angulosRadarF[] = {50,65,85,100,115,135,150};
//{30,40,50,60,70,80,90,100,110,120,130,140,150,160,170};
const byte cantidadRadarF = sizeof(angulosRadarF);
int medidasRadarF[cantidadRadarF+1];

byte angulosRadarD[] = {100,110,120,130,140,150};
//{30,40,50,60,70,80,90,100,110,120,130,140,150,160,170};
const byte cantidadRadarD = sizeof(angulosRadarD);
int medidasRadarD[cantidadRadarD+1];

bool radarActivado = false;

// -------------------- variables funciones de evasion de objetos cercanos
bool obstaculo = false;
int dist_maxi_fron = 80;       // distancia maxima frontal, seguidor
int dist_mini_fron = 50;       // distancia minima frontal 
int dist_mini_late = 20;       // distancia minima lateral
int dist_maxi_late = 60;       // distancia maxima lateral, seguidor

// -------------------- variables funciones de control de tiempo de muestreo
unsigned long tiempoUltimaMedicionDistancia = 0;          // variable auxiliar para tener un registro del tiempo donde se midio por ultima vez
unsigned int tiempoMinimoEntreMedicionDistancia = 20;     // tiempo en milisegundos entre actualizacion de la medicion del ultraosonido 1 al 3

#define SensorUltrasonidoIzquierdo  0
#define SensorUltrasonidoFrontal    1
#define SensorUltrasonidoDerecho    2
byte ultrasonidoMedido = SensorUltrasonidoIzquierdo;
bool medirTodosUltrasonidos = false;
int valorMaximoMedicionDistancia = 500;

// variables usadas para saber si hay una medicion completa y para relaizar la conversion
bool medidaCompletaI = false;
bool medidaCompletaF = false;
bool medidaCompletaD = false;
// varible usada para saver si hay datos convertidos completos de la ultima medicion
bool conversionCompletaI = false;
bool conversionCompletaF = false;
bool conversionCompletaD = false;

// variables para detectar la desconexion del sensor
byte medidaFallidaI = 0;
byte medidaFallidaF = 0;
byte medidaFallidaD = 0;
bool ultrasonidoConectadoI = true;
bool ultrasonidoConectadoF = true;
bool ultrasonidoConectadoD = true;



void actulizarSensoresDistancia(){
   if(millis() > tiempoUltimaMedicionDistancia + tiempoMinimoEntreMedicionDistancia){
    
    // si estoy en depuracion informo el estado
    //if(depuracion)InformarEstadoDistancias();
    revisarConexionDistancia();
    calcularDistanciaCentimetros();             // ajustar para que solo se calcule cuando halla una medicion valida
    
    switch(ultrasonidoMedido){
      case SensorUltrasonidoIzquierdo     :

        if(radarActivado){
          actulizarRadarI();
        }else{
          sensorUltrasonidoI();
        }
        
      break;case SensorUltrasonidoFrontal :

        if(radarActivado){
          actulizarRadarF();
        }else{
          sensorUltrasonidoF();
        }
        
      break;case SensorUltrasonidoDerecho :

        if(radarActivado){
          actulizarRadarD();
        }else{
          sensorUltrasonidoD();
        }
        
      break;
    }
    //if(medirTodosUltrasonidos)ultrasonidoMedido = (ultrasonidoMedido+1)%3;    // salto al siguiente sensor
    ultrasonidoMedido = (ultrasonidoMedido+1)%3;    // salto al siguiente sensor
    // actualizo el ultimo tiempo de medicion
    tiempoUltimaMedicionDistancia = millis();
   }
}


void actulizarRadarI(){
  if(conversionCompletaI){
    // mandamos el servo a la siguiente posicion
    medidasRadarI[posicionRadarI] = medidaDistanciaIzquierda;
    servoI.write(angulosRadarI[posicionRadarI]);

    
    if (posicionRadarI >= cantidadRadarI-1){direccionRadarCrecienteI = false;}
    if (posicionRadarI <= 0){direccionRadarCrecienteI = true;}
    if (direccionRadarCrecienteI){
      posicionRadarI ++; // aumentamos la posicion
    }else{
      posicionRadarI --;  // disminuimos la posicion  
    }
    
  }
  sensorUltrasonidoI();
}

void actulizarRadarF(){
  if(conversionCompletaF){
    // mandamos el servo a la siguiente posicion
    medidasRadarF[posicionRadarF] = medidaDistanciaFrontal;
    servoF.write(angulosRadarF[posicionRadarF]);
      
    if (posicionRadarF >= cantidadRadarF-1){direccionRadarCrecienteF = false;}
    if (posicionRadarF <= 0){direccionRadarCrecienteF = true;}
    if (direccionRadarCrecienteF){
      posicionRadarF ++; // aumentamos la posicion
    }else{
      posicionRadarF --;  // disminuimos la posicion  
    }
  }
  sensorUltrasonidoF();
}

void actulizarRadarD(){
  if(conversionCompletaD){
    // mandamos el servo a la siguiente posicion
    medidasRadarD[posicionRadarD] = medidaDistanciaDerecha;
    servoD.write(angulosRadarD[posicionRadarD]);
      
    if (posicionRadarD >= cantidadRadarD-1){direccionRadarCrecienteD = false;}
    if (posicionRadarD <= 0){direccionRadarCrecienteD = true;}
    if (direccionRadarCrecienteD){
      posicionRadarD ++; // aumentamos la posicion
    }else{
      posicionRadarD --;  // disminuimos la posicion  
    }
  }
  sensorUltrasonidoD();
}


bool ObstaculoRadarI(){
  bool hayObstaculo =  false; // supongo que no hay obstaculos
  for(int i=0;i<cantidadRadarI-1;i++){
    if (medidasRadarI[i] < dist_mini_late){
      hayObstaculo = true;   // si hay objetos muy cercanos los evado y informo
      return hayObstaculo;
    }
  }
  return hayObstaculo;
}

bool ObstaculoRadarF(){
  bool hayObstaculo =  false; // supongo que no hay obstaculos
  for(int i=0;i<cantidadRadarF-1;i++){
    if (medidasRadarF[i] < dist_mini_fron){
      hayObstaculo = true;   // si hay objetos muy cercanos los evado y informo
      return hayObstaculo;
    }
  }
  return hayObstaculo;
}

bool ObstaculoRadarD(){
  bool hayObstaculo =  false; // supongo que no hay obstaculos
  for(int i=0;i<cantidadRadarD-1;i++){
    if (medidasRadarD[i] < dist_mini_late){
      hayObstaculo = true;   // si hay objetos muy cercanos los evado y informo
      return hayObstaculo;
    }
  }
  return hayObstaculo;
}


void informarEstadoMedicionRadares(){
  radarActivado = true;
  informarEstadoMedicionRadarI();
  informarEstadoMedicionRadarF();
  informarEstadoMedicionRadarD();
}

void informarEstadoMedicionRadarI(){
  // medicion radar ultrasonido
  Serial.print(posicionRadarI);
  Serial.print("\t");
  for(int i=0;i<cantidadRadarI-1;i++){
    Serial.print(medidasRadarI[i]);
    Serial.print("\t");
  }
  Serial.print("I");
  Serial.print("\t");
  Serial.println();
  delay(100);  // tiempo minimo que toca esperar para evitar saturar el buffer de salida, minimo -> 6
}

void informarEstadoMedicionRadarF(){
  // medicion radar ultrasonido
  Serial.print(posicionRadarF);
  Serial.print("\t");
  for(int i=0;i<cantidadRadarF-1;i++){
    Serial.print(medidasRadarF[i]);
    Serial.print("\t");
  }
  Serial.print("F");
  Serial.print("\t");
  Serial.println();
  delay(100);  // tiempo minimo que toca esperar para saturar el buffer de salida, minimo -> 6
}

void informarEstadoMedicionRadarD(){
  // medicion radar ultrasonido
  Serial.print(posicionRadarD);
  Serial.print("\t");
  for(int i=0;i<cantidadRadarD-1;i++){
    Serial.print(medidasRadarD[i]);
    Serial.print("\t");
  }
  Serial.print("D");
  Serial.print("\t");
  Serial.println();
  delay(100);  // tiempo minimo que toca esperar para saturar el buffer de salida, minimo -> 6
}


void limpiarMedidasRadares(){
  limpiarMedidasRadarI();
  limpiarMedidasRadarF();
  limpiarMedidasRadarD();
}

void limpiarMedidasRadarI(){
  for(int i=0;i<cantidadRadarI-1;i++){
    medidasRadarI[i] = 500;
  }
}

void limpiarMedidasRadarF(){
  for(int i=0;i<cantidadRadarF-1;i++){
    medidasRadarF[i] = 500;
  }
}

void limpiarMedidasRadarD(){
  for(int i=0;i<cantidadRadarD-1;i++){
    medidasRadarD[i] = 500;
  }
}


// funcion para medir los ultrasonidos
void sensorUltrasonidoI(){
  // se manda un pulso de 10us
  pinMode(pinSensorUltrasonidoI, OUTPUT);
  digitalWrite(pinSensorUltrasonidoI, HIGH);
  delayMicroseconds(10);
  
  // se activa una interrupcion para medir el echo
  pinMode(pinSensorUltrasonidoI, INPUT);
  medidaCompletaI = false;
  conversionCompletaI = false;
  attachInterrupt(pinSensorUltrasonidoI, funcionFlancoSubidaI, RISING);
}

void funcionFlancoSubidaI(){
  tiempoMedicionPulsoAltoI = micros();
  attachInterrupt(pinSensorUltrasonidoI, funcionFlancoBajadaI, FALLING);
}

void funcionFlancoBajadaI(){
  tiempoPulsoAltoI = micros()-tiempoMedicionPulsoAltoI;
  medidaCompletaI = true;
  detachInterrupt(pinSensorUltrasonidoI);     //terminado el proceso de medicion, se desactiva la interrupcion
}


void sensorUltrasonidoF(){
  // se manda un pulso de 10us
  pinMode(pinSensorUltrasonidoF, OUTPUT);
  digitalWrite(pinSensorUltrasonidoF, HIGH);
  delayMicroseconds(10);
  
  // se activa una interrupcion para medir el echo
  pinMode(pinSensorUltrasonidoF, INPUT);
  medidaCompletaF = false;
  conversionCompletaF = false;
  attachInterrupt(pinSensorUltrasonidoF, funcionFlancoSubidaF, RISING);
}

void funcionFlancoSubidaF(){
  tiempoMedicionPulsoAltoF = micros();
  attachInterrupt(pinSensorUltrasonidoF, funcionFlancoBajadaF, FALLING);
}

void funcionFlancoBajadaF(){
  tiempoPulsoAltoF = micros()-tiempoMedicionPulsoAltoF;
  medidaCompletaF = true;
  detachInterrupt(pinSensorUltrasonidoF);     //terminado el proceso de medicion, se desactiva la interrupcion
}


void sensorUltrasonidoD(){
  // se manda un pulso de 10us
  pinMode(pinSensorUltrasonidoD, OUTPUT);
  digitalWrite(pinSensorUltrasonidoD, HIGH);
  delayMicroseconds(10);
  
  // se activa una interrupcion para medir el echo
  pinMode(pinSensorUltrasonidoD, INPUT);
  medidaCompletaD = false;
  conversionCompletaD = false;
  attachInterrupt(pinSensorUltrasonidoD, funcionFlancoSubidaD, RISING);
}

void funcionFlancoSubidaD(){
  tiempoMedicionPulsoAltoD = micros();
  attachInterrupt(pinSensorUltrasonidoD, funcionFlancoBajadaD, FALLING);
}

void funcionFlancoBajadaD(){
  tiempoPulsoAltoD = micros()-tiempoMedicionPulsoAltoD;
  medidaCompletaD = true;
  detachInterrupt(pinSensorUltrasonidoD);     //terminado el proceso de medicion, se desactiva la interrupcion
}


void MostrarUltrasonido(char dato){
  byte estado = Detenido;
  // modificamos el movimiento
  switch(dato){
    case '1'        : medirTodosUltrasonidos = false;ultrasonidoMedido = SensorUltrasonidoIzquierdo;
    break;case '2'  : medirTodosUltrasonidos = false;ultrasonidoMedido = SensorUltrasonidoFrontal;
    break;case '3'  : medirTodosUltrasonidos = false;ultrasonidoMedido = SensorUltrasonidoDerecho;
    break;case '4'  : medirTodosUltrasonidos = true;
    break;
  }
  InicializarMedicionesUltrasonidos();
  desactivarInterrupcionesUltrasonidos();
}

void InicializarMedicionesUltrasonidos(){
  tiempoMedicionPulsoAltoI = tiempoMedicionPulsoAltoF = tiempoMedicionPulsoAltoD = 0;
  tiempoPulsoAltoI = tiempoPulsoAltoF = tiempoPulsoAltoD = 0;
  medidaDistanciaDerecha = medidaDistanciaFrontal = medidaDistanciaIzquierda = 0;
}

void desactivarInterrupcionesUltrasonidos(){
  detachInterrupt(pinSensorUltrasonidoI);
  detachInterrupt(pinSensorUltrasonidoF);
  detachInterrupt(pinSensorUltrasonidoD);
}


void informarMedicionDetalladaDistancia(){
    Serial.print(tiempoMedicionPulsoAltoI);
    Serial.print("\t");
    Serial.print(tiempoPulsoAltoI);
    Serial.print("\t");
    Serial.print(medidaDistanciaIzquierda);
    Serial.print("\t");               

    Serial.print(tiempoMedicionPulsoAltoF);
    Serial.print("\t");
    Serial.print(tiempoPulsoAltoF);
    Serial.print("\t");
    Serial.print(medidaDistanciaFrontal);
    Serial.print("\t");              
         
    Serial.print(tiempoMedicionPulsoAltoD);
    Serial.print("\t");
    Serial.print(tiempoPulsoAltoD);
    Serial.print("\t");
    Serial.print(medidaDistanciaDerecha);
     
    Serial.println();
    delay(20);
}

void informarMedicionDistancia(){
    radarActivado = false;
    if(conversionCompletaI)Serial.print(millis());
    else Serial.print(0);
    Serial.print("\t");
    Serial.print(medidaDistanciaIzquierda);
    Serial.print("\t");
                   
    if(conversionCompletaF)Serial.print(millis());
    else Serial.print(0);
    Serial.print("\t");
    Serial.print(medidaDistanciaFrontal);
    Serial.print("\t");
                  
    if(conversionCompletaD)Serial.print(millis());
    else Serial.print(0);
    Serial.print("\t");
    Serial.print(medidaDistanciaDerecha);
     
    Serial.println();
    delay(60);
}

void informarMedicionDistanciaBasica(){
    Serial.print(medidaDistanciaIzquierda);
    Serial.print("\t");
    
    Serial.print(medidaDistanciaFrontal);
    Serial.print("\t");
    
    Serial.print(medidaDistanciaDerecha);
     
    Serial.println();
    delay(60);
}


void activarSensoresDistancia(){
  servoI.attach(7);  // asigno pin al servo
  servoD.attach(5);  // asigno pin al servo
  servoF.attach(6);  // asigno pin al servo
  limpiarMedidasRadares();
}

void posicionFijaRadares(){
  if(ver == EvasorObstaculos){
    servoI.write(60);
    servoF.write(105);
    servoD.write(125);
    radarActivado = false;
  }
  if(ver == SeguidorObjetos){
    servoI.write(35);
    servoF.write(105);
    servoD.write(155);
    radarActivado = false;
  }
}


void revisarConexionDistancia(){
  bool ultrasonidoAnteriorConectadoI = ultrasonidoConectadoI;
  if(medidaFallidaI>5){
    ultrasonidoConectadoI = false;
    medidaFallidaI = 10;    // evitamos que se desvorde
  }else{
    ultrasonidoConectadoI = true;
  }
  if(depuracion && ultrasonidoAnteriorConectadoI != ultrasonidoConectadoI)InformarEstadoConexionSensoresDistanciaI();
  bool ultrasonidoAnteriorConectadoF = ultrasonidoConectadoF;
  if(medidaFallidaF>5){
    ultrasonidoConectadoF = false;
    medidaFallidaF = 10;    // evitamos que se desvorde
  }else{
    ultrasonidoConectadoF = true;
  }
  if(depuracion && ultrasonidoAnteriorConectadoF != ultrasonidoConectadoF)InformarEstadoConexionSensoresDistanciaF();
  bool ultrasonidoAnteriorConectadoD = ultrasonidoConectadoD;
  if(medidaFallidaD>5){
    ultrasonidoConectadoD = false;
    medidaFallidaD = 10;    // evitamos que se desvorde
  }else{
    ultrasonidoConectadoD = true;
  }
  if(depuracion && ultrasonidoAnteriorConectadoD != ultrasonidoConectadoD)InformarEstadoConexionSensoresDistanciaD();
}

void InformarEstadoConexionSensoresDistanciaI(){
  Serial.print("Sensor Distancia Izquierdo ");
  if(ultrasonidoConectadoI){
    Serial.println("Conectado");
  }else{
    Serial.println("Desconectado");
  }
}

void InformarEstadoConexionSensoresDistanciaF(){
  Serial.print("Sensor Distancia Frontal ");
  if(ultrasonidoConectadoF){
    Serial.println("Conectado");
  }else{
    Serial.println("Desconectado");
  }
}

void InformarEstadoConexionSensoresDistanciaD(){
  Serial.print("Sensor Distancia Derecho ");
  if(ultrasonidoConectadoD){
    Serial.println("Conectado");
  }else{
    Serial.println("Desconectado");
  }
}


void calcularDistanciaCentimetros(){
  calcularDistanciaCentimetrosI();
  calcularDistanciaCentimetrosF();
  calcularDistanciaCentimetrosD();
}

void calcularDistanciaCentimetrosI(){
  if(medidaCompletaI){
    int aux = medidaDistanciaIzquierda;
    medidaDistanciaIzquierda = tiempoPulsoAltoI/58.30;
    if(medidaDistanciaIzquierda > valorMaximoMedicionDistancia){
      medidaDistanciaIzquierda = aux;
    }else{
      conversionCompletaI = true;
      medidaFallidaI = 0;
    }
  }else{
    medidaFallidaI++;
  }
}

void calcularDistanciaCentimetrosF(){
  if(medidaCompletaF){
    int aux = medidaDistanciaFrontal;
    medidaDistanciaFrontal = tiempoPulsoAltoF/58.30;
    if(medidaDistanciaFrontal > valorMaximoMedicionDistancia){
      medidaDistanciaFrontal = aux;
    }else{
      conversionCompletaF = true;
      medidaFallidaF = 0;
    }
  }else{
    medidaFallidaF++;
  }
}

void calcularDistanciaCentimetrosD(){
  if(medidaCompletaD){
    int aux = medidaDistanciaDerecha;
    medidaDistanciaDerecha = tiempoPulsoAltoD/58.30;
    if(medidaDistanciaDerecha > valorMaximoMedicionDistancia){
      medidaDistanciaDerecha = aux;
    }else{
      conversionCompletaD = true;
      medidaFallidaD = 0;
    }
  }else{
    medidaFallidaD++;
  }
}





