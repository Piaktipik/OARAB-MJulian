
// ************************************** Sensores Contacto - (Fines de Carrera) ************************************** 
// -------------------- puertos usados del arduino
const byte pinSensoresColision[] = {47,46,44,45,9,8,4,3};    // pines digitales en orden del frontal hacia la izquierda
// variables medicion
unsigned long tiempoUltimaMedicionColision = 0;        // variable auxiliar para tener un registro del tiempo donde se midio por ultima vez
unsigned int tiempoMinimoEntreMedicionColision = 100;  // tiempo en milisegundos entre actualizacion del estado de no deteccion y impresion de un nuevo estado

// variables para el manejo del estado de las colisiones
#define NoColision 0 
//#define MidiendoColision 1
#define ColisionFrontal 11
#define ColisionFrontalI 12
#define ColisionIzquierda 13
#define ColisionTraseraI 14
#define ColisionTrasera 15
#define ColisionTraseraD 16
#define ColisionDerecha 17
#define ColisionFrontalD 18
byte estadoColisionActual = NoColision;
byte estadoColisionAnterior;  // variable para evitar imprimir muchas veces un estado que no ha cambiado

// modificar para detectar colisiones con interrupciones
void activarSesoresColisiones(){
  for (int i=0;i<8;i++){
    pinMode(pinSensoresColision[i],INPUT); // se ponen como entradas
  }
  // se activan las interrupciones, para detectar las colisiones
  attachInterrupt(pinSensoresColision[0], iColisionFontal,RISING);
  attachInterrupt(pinSensoresColision[1], iColisionFontalI,RISING);
  attachInterrupt(pinSensoresColision[2], iColisionIzquierda,RISING);
  attachInterrupt(pinSensoresColision[3], iColisionTraseraI,RISING);
  attachInterrupt(pinSensoresColision[4], iColisionTrasera,RISING);
  attachInterrupt(pinSensoresColision[5], iColisionTraseraD,RISING);
  attachInterrupt(pinSensoresColision[6], iColisionDerecha,RISING);
  attachInterrupt(pinSensoresColision[7], iColisionFontalD,RISING);
}
  


void actulizarSensoresColision(){
  if(millis() > tiempoUltimaMedicionColision + tiempoMinimoEntreMedicionColision){
    
    // si estoy en depuracion informo el estado
    if(depuracion)InformarEstadoColision();
    
    if(!hayColision()){
      estadoColisionActual = NoColision;
    }
    // orden de prioridad de la deteccion menos a mas prioridad
    //colisionTraseraI();
    //colisionTraseraD();
    //colisionFontalI();
    //colisionFontalD();
    //colisionIzquierda();
    //colisionDerecha();
    //colisionTrasera();
    //colisionFontal();
    
    // actualizo el ultimo tiempo de medicion
    tiempoUltimaMedicionColision = millis();
  }
}

bool hayColision(){
  bool aux = false;
  for (int i=0;i<8;i++){
      aux = (aux || digitalRead(pinSensoresColision[i]));
   }
   return aux;
}

// interrupcion de colisiones
void iColisionFontal(){
  estadoColisionActual = ColisionFrontal;
}
void iColisionFontalI(){
  estadoColisionActual = ColisionFrontalI;
}
void iColisionIzquierda(){
  estadoColisionActual = ColisionIzquierda;
}
void iColisionTraseraI(){
  estadoColisionActual = ColisionTraseraI;
}
void iColisionTrasera(){
  estadoColisionActual = ColisionTrasera;
}
void iColisionTraseraD(){
  estadoColisionActual = ColisionTraseraD;
}
void iColisionDerecha(){
  estadoColisionActual = ColisionDerecha;
}
void iColisionFontalD(){
  estadoColisionActual = ColisionFrontalD;
}

void InformarEstadoColision(){
  // si el estado cambio lo informo
  if(estadoColisionActual != estadoColisionAnterior){
    if(estadoColisionActual == ColisionFrontal){Serial.println("Colision Frontal Detectada");
    }else if (estadoColisionActual == ColisionFrontalI){Serial.println("Colision Frontal Izquierda Detectada");
    }else if (estadoColisionActual == ColisionIzquierda){Serial.println("Colision Izquierda Detectada");
    }else if (estadoColisionActual == ColisionTraseraI){Serial.println("Colision Trasera Izquierda Detectada");
    }else if (estadoColisionActual == ColisionTrasera){Serial.println("Colision Trasera Detectada");
    }else if (estadoColisionActual == ColisionTraseraD){Serial.println("Colision Trasera Derecha Detectada");
    }else if (estadoColisionActual == ColisionDerecha){Serial.println("Colision Derecha Detectada");
    }else if (estadoColisionActual == ColisionFrontalD){Serial.println("Colision Frontal Derecha Detectada");
    //}else if (estadoColisionActual == MidiendoColision){Serial.println("Midiendo Colision");
    }else if (estadoColisionActual == NoColision){Serial.println("No Colisiones");
    }
    estadoColisionAnterior = estadoColisionActual; // almacenamos el ultimo estado 
  }
}

void muestreoSimpleColision(){
    for (int i=0;i<8;i++){
      Serial.print(digitalRead(pinSensoresColision[i]));
      Serial.print(" ");
    }
    Serial.println();
}

// colsulta de colisiones individuales
bool colisionFontalI(){
  if(digitalRead(pinSensoresColision[1])){
    estadoColisionActual = ColisionFrontalI;
    return true;
  }else{
    return false;
  }
}
bool colisionIzquierda(){
  if(digitalRead(pinSensoresColision[2])){
    estadoColisionActual = ColisionIzquierda;
    return true;
  }else{
    return false;
  }
}
bool colisionTraseraI(){
  if(digitalRead(pinSensoresColision[3])){
    estadoColisionActual = ColisionTraseraI;
    return true;    
  }else{
    return false;
  }
}
bool colisionTrasera(){
  if(digitalRead(pinSensoresColision[4])){
    estadoColisionActual = ColisionTrasera;
    return true;
  }else{
    return false;
  }
}
bool colisionTraseraD(){
  if(digitalRead(pinSensoresColision[5])){
    estadoColisionActual = ColisionTraseraD;
    return true;    
  }else{
    return false;
  }
}
bool colisionDerecha(){
  if(digitalRead(pinSensoresColision[6])){
    estadoColisionActual = ColisionDerecha;
    return true;    
  }else{
    return false;
  }
}
bool colisionFontalD(){
  if(digitalRead(pinSensoresColision[7])){
    estadoColisionActual = ColisionFrontalD;
    return true;    
  }else{
    return false;
  }
}
