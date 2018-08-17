// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de pista zzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// -------------------- puertos usados del arduino
// izquierda
const byte sen_lineae1 = 43;                            // pin de abilitacion mosfet
const byte sen_linea1[] = {41,39,37,35,33,31,29,27};    // pines digitales del sensor en orden
// derecho
const byte sen_lineae2 = 42;                            // pin de abilitacion mosfet
const byte sen_linea2[] = {40,38,36,34,32,30,28,26};    // pines digitales del sensor

// -------------------- variables generales
const byte nsen = 8;                                    // numero de sensores de infrarrojo por modulo

// -------------------- variables tiempos medidos de los sensores
// izquierda
unsigned long tiem_refl_1[nsen+1];                        // variable para calculo de la medida de la reflexion
unsigned long medi_refl_1[nsen+1];                         // contiene la medida de la relfexion
// derecho
unsigned long tiem_refl_2[nsen+1];                        // variable para calculo de la medida de la reflexion
unsigned long medi_refl_2[nsen+1];                         // contiene la medida de la relfexion

// -------------------- variables calibracion
int rlinea = 500;                
//  variables usadas para comparar cada sensor de linea individualmente
int valo_refe_line1[] = {rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea};
int valo_refe_line2[] = {rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea};

// -------------------- variables para procesamiento mediciones
byte cont_line_dere = 0;                                // variable conteo sensores de linea derechos, detectando linea
byte cont_line_izqu = 0;                                // variable conteo sensores de linea izquierdo, detectando linea

bool veo_line_fren = false;                             // se pone en alto cuando se detecta linea de frente
bool veo_line_late = false;                             // se pone en alto cuando se detecta linea de lateralmente

bool medi_bool_line[16];                                // vector con los valores booleanos de los sensores -> detecta o no

// -------------------- variables muestreo
unsigned long tiempoUltimaMedicionSuelo;   // tiempo de medicion infrarrojos -> guardo el tiempo desde la ultima medicion
unsigned int tiempoMinimoEntreMedicionSuelo = 50; //tiempo en milisegundos minimo entre mediciones

// se usa para saber cuando se han medido todos los sensores para apagar los leds
byte contadorMedicionesI = 9;   //suponemos que midieron algo, antes de verificiar si estan conectados   
byte contadorMedicionesD = 9;
   
// verificar respuesta de los sensores, para determinar que estan o no conectados
bool sensorSueloConectadoI = true;
bool sensorSueloConectadoD = true;

// variable para verificar que no hay suelo adelante, el robot se levanto o hay un linea negra bajo los sensores
bool haySueloAdelante = false;

void actualizarSensoresSuelo(){
  // --------- mido los sensores de pista
  if(millis() > tiempoUltimaMedicionSuelo + tiempoMinimoEntreMedicionSuelo){
    revisarConexionLinea();
    obtenerMedicionBoleanaSuelo();
    medirSuelo();
    tiempoUltimaMedicionSuelo = millis(); // actualizo el contador
  }
}

void medirSuelo(){
  // enciendo leds
  digitalWrite(sen_lineae1,1);
  digitalWrite(sen_lineae2,1);
  // cargo los condesandores poniendo en alto los pines
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],OUTPUT);digitalWrite(sen_linea1[i],HIGH);
    pinMode(sen_linea2[i],OUTPUT);digitalWrite(sen_linea2[i],HIGH);
  }
  // espero 10us a que cargue el capacitor
  delayMicroseconds(10);
  // pongo los pines como entrada y espero a la interrupcion
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],INPUT);tiem_refl_1[i] = micros();
    pinMode(sen_linea2[i],INPUT);tiem_refl_2[i] = micros();
  }
  // activo las interupciones de los de pista s1
  attachInterrupt(sen_linea1[0],s1p0,FALLING);
  attachInterrupt(sen_linea1[1],s1p1,FALLING);
  attachInterrupt(sen_linea1[2],s1p2,FALLING);
  attachInterrupt(sen_linea1[3],s1p3,FALLING);
  attachInterrupt(sen_linea1[4],s1p4,FALLING);
  attachInterrupt(sen_linea1[5],s1p5,FALLING);
  attachInterrupt(sen_linea1[6],s1p6,FALLING);
  attachInterrupt(sen_linea1[7],s1p7,FALLING);
  // activo las interupciones de los de pista s2
  attachInterrupt(sen_linea2[0],s2p0,FALLING);
  attachInterrupt(sen_linea2[1],s2p1,FALLING);
  attachInterrupt(sen_linea2[2],s2p2,FALLING);
  attachInterrupt(sen_linea2[3],s2p3,FALLING);
  attachInterrupt(sen_linea2[4],s2p4,FALLING);
  attachInterrupt(sen_linea2[5],s2p5,FALLING);
  attachInterrupt(sen_linea2[6],s2p6,FALLING);
  attachInterrupt(sen_linea2[7],s2p7,FALLING);
  contadorMedicionesI = 0;
  contadorMedicionesD = 0;
}

void s1p0(){medi_refl_1[0] = micros()-tiem_refl_1[0];contadorMedicionesD++;}
void s1p1(){medi_refl_1[1] = micros()-tiem_refl_1[1];contadorMedicionesD++;}
void s1p2(){medi_refl_1[2] = micros()-tiem_refl_1[2];contadorMedicionesD++;}
void s1p3(){medi_refl_1[3] = micros()-tiem_refl_1[3];contadorMedicionesD++;}
void s1p4(){medi_refl_1[4] = micros()-tiem_refl_1[4];contadorMedicionesD++;}
void s1p5(){medi_refl_1[5] = micros()-tiem_refl_1[5];contadorMedicionesD++;}
void s1p6(){medi_refl_1[6] = micros()-tiem_refl_1[6];contadorMedicionesD++;}
void s1p7(){medi_refl_1[7] = micros()-tiem_refl_1[7];contadorMedicionesD++;}

void s2p0(){medi_refl_2[0] = micros()-tiem_refl_2[0];contadorMedicionesI++;}
void s2p1(){medi_refl_2[1] = micros()-tiem_refl_2[1];contadorMedicionesI++;}
void s2p2(){medi_refl_2[2] = micros()-tiem_refl_2[2];contadorMedicionesI++;}
void s2p3(){medi_refl_2[3] = micros()-tiem_refl_2[3];contadorMedicionesI++;}
void s2p4(){medi_refl_2[4] = micros()-tiem_refl_2[4];contadorMedicionesI++;}
void s2p5(){medi_refl_2[5] = micros()-tiem_refl_2[5];contadorMedicionesI++;}
void s2p6(){medi_refl_2[6] = micros()-tiem_refl_2[6];contadorMedicionesI++;}
void s2p7(){medi_refl_2[7] = micros()-tiem_refl_2[7];contadorMedicionesI++;}

void apagarLedsI(){
  if (contadorMedicionesD >= 8){
    // apago leds -- se dejan encendidos por estabilidad hasta realizar pruebas con osciloscopio
    //digitalWrite(sen_lineae1,0);
  }
}

void apagarLedsD(){
  if (contadorMedicionesI >= 8){
    // apago leds -- se dejan encendidos por estabilidad hasta realizar pruebas con osciloscopio
    //digitalWrite(sen_lineae2,0);
  } 
}


void obtenerMedicionBoleanaSuelo(){
  cont_line_dere = 0;     // variable conteo sensores de linea derechos detectando linea
  cont_line_izqu = 0;     // variable conteo sensores de linea izquierdos detectando linea
  
  for(int i=0;i<nsen;i++){
    // convierto en boleano y cuento las detecciones del lado derecho
    if(medi_refl_1[i] < valo_refe_line1[i]){
      medi_bool_line[i]=HIGH;
      cont_line_dere++;
    }else{
      medi_bool_line[i]=LOW;
    }
    // convierto en boleano y cuento las detecciones del lado izquierdo
    if(medi_refl_2[i] < valo_refe_line2[i]){
      medi_bool_line[nsen+i]=HIGH;
      cont_line_izqu++;
    }else{
      medi_bool_line[nsen+i]=LOW;
    }
  }
  //if(depuracion)informarEstadoMedicionSuelo();
}

void revisarLinea(){
  
  obtenerMedicionBoleanaSuelo();
  
  if(cont_line_dere > 0 && cont_line_izqu > 0){veo_line_fren = true;veo_line_late = false;}        // si amos modulos detectan, tengo una linea en frente
  else if(cont_line_dere > 1 || cont_line_izqu > 1){veo_line_late = true;veo_line_fren = false;}   // si solo un modulo detecta, tengo una linea a un lado
}

bool haySuelo(){
  bool habiaSuelo = haySueloAdelante;
  if(cont_line_dere + cont_line_izqu > 0 && (sensorSueloConectadoI && sensorSueloConectadoD)){
    haySueloAdelante = true;
  }else{
    haySueloAdelante = false;
  }
  if(depuracion && (habiaSuelo != haySueloAdelante))informarEstadoSuelo();
  return haySueloAdelante;
}


void activarSensoresSuelo(){
  // declaro los puertos de control de los mosfet de los leds como salidas
  pinMode(sen_lineae1,OUTPUT);
  pinMode(sen_lineae2,OUTPUT);
}


void revisarConexionLinea(){
  bool sensorSueloAnteriorConectadoI = sensorSueloConectadoI;
  if (contadorMedicionesI < 8){
    sensorSueloConectadoI = false;
    limpiarMedicionI();
  }else{
    sensorSueloConectadoI = true;
  }
  if(depuracion && sensorSueloAnteriorConectadoI != sensorSueloConectadoI)InformarEstadoConexionSensoresSueloI();
  bool sensorSueloAnteriorConectadoD = sensorSueloConectadoD;
  if (contadorMedicionesD < 8){
    sensorSueloConectadoD = false;
    limpiarMedicionD();
  }else{
    sensorSueloConectadoD = true;
  }
  if(depuracion && sensorSueloAnteriorConectadoD != sensorSueloConectadoD)InformarEstadoConexionSensoresSueloD();
}

void limpiarMedicionD(){
  for(int i=0;i<nsen;i++){
    medi_refl_1[i] = valo_refe_line1[i]+1;
  }
}

void limpiarMedicionI(){
  for(int i=0;i<nsen;i++){
    medi_refl_2[i] = valo_refe_line2[i]+1;
  }
}

void InformarEstadoConexionSensoresSueloI(){
  if(sensorSueloConectadoI){
    Serial.println("Sensor Suelo Izquierdo Conectado");
  }else{
    Serial.println("Sensor Suelo Izquierdo Desconectado");
  }
  //delay(5);
}

void InformarEstadoConexionSensoresSueloD(){
  if(sensorSueloConectadoD){
    Serial.println("Sensor Suelo Derecho Conectado");
  }else{
    Serial.println("Sensor Suelo Derecho Desconectado");
  }
  //delay(5);
}

void informarEstadoSuelo(){
  if(haySueloAdelante){
    Serial.println("Suelo Detectado");
  }else{
    Serial.println("Suelo No Detectado");
  }
}

void informarEstadoMedicionSuelo(){
  // prueba infrarrojos de pista
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_1[i]);
    Serial.print("\t");
  }
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_2[i]);
    Serial.print("\t");
  }
  Serial.print(" | ");
  for(int i=0;i<16;i++){ 
    Serial.print(medi_bool_line[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(30);  // tiempo minimo que toca esperar para saturar el buffer de salida, minimo -> 6
}

