
// ************************************** Encoder2 Motor Derecho - (Magnetico) **************************************
// -------------------- puertos usados del arduino
// ----- encoder 2 -----
int pinA2 = 24;
int pinB2 = 25;

// -------------------- Variables para la medicion
// ----- encoder 2 -----
volatile unsigned int ca2 = 0; volatile unsigned int cb2 = 0;    // contador de entrada a la interrupcion
volatile int est_enc2 = 0;                              // estado del encoder {1,2,3,4}
volatile long pas_tota2 = 0;                             // pasos absolutos del encoder

// -------------------- Variables para la medicion tiempo entre pulsos
volatile unsigned long tiempoDesdeAnteriorPulsoA2 = 0;
volatile unsigned long tiempoDesdeAnteriorPulsoB2 = 0;
volatile unsigned long deltaTiempoPulsoA2 = 0;
volatile unsigned long deltaTiempoPulsoB2 = 0;

//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ encoders  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void activarEncoder2(int pA, int pB){
  // se ponen como entradas los puertos del encoder
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  // se incializan las bases de tiempo
  tiempoDesdeAnteriorPulsoA2 = 0;
  tiempoDesdeAnteriorPulsoB2 = 0;
  deltaTiempoPulsoA2 = 0;
  deltaTiempoPulsoB2 = 0;
  // se verifica el estado 
  est_enc2 = obt_est2(pA,pB);
  // 
  esp_pro_est2(pA, pB, est_enc2);
}

void ENCODERA2() {
  if(calcularDeltaTiempoPulsos){
    // obtengo el delta de tiempo entre flancos
    deltaTiempoPulsoA2 = abs(micros() - tiempoDesdeAnteriorPulsoA2);
    tiempoDesdeAnteriorPulsoA2 = micros();
  }
  pro_est_A2(pinA2,pinB2,est_enc2);
  // aumento el contador de pasos
  ca2++;
}

void ENCODERB2() {
  if(calcularDeltaTiempoPulsos){
    // obtengo el delta de tiempo entre flancos
    deltaTiempoPulsoB2 = abs(micros() - tiempoDesdeAnteriorPulsoB2);
    tiempoDesdeAnteriorPulsoB2 = micros();
  }
  pro_est_B2(pinA2,pinB2,est_enc2);
  // aumento el contador de pasos
  cb2++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int obt_est2(int pA,int pB){
  int x = digitalRead(pA)*2 + digitalRead(pB);
  switch(x){
    case 0:
      x = 1;
      break;
    case 2:
      x = 2;
      break;
    case 3:
      x = 3;
      break;
    case 1:
      x = 4;
      break;
  }
  return x; 
}

void esp_pro_est2(int pA,int pB,int est_enc){
  switch(est_enc2){
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, FALLING);
      break;
  }
}

int pro_est_A2(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 2;
      break;
    case 2:
      x = 1;
      break;
    case 3:
      x = 4;
      break;
    case 4:
      x = 3;
      break;
  }
  est_enc2 = x;
  esp_pro_est2(pA,pB,x);
}

void pro_est_B2(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 4;
      pas_tota2--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_tota2++;
      break;
  }
  est_enc2 = x;
  esp_pro_est2(pA,pB,x);
}

void informarEstadoMedicionEncoders(){
    Serial.print("Ca1= " + String(ca1));
    Serial.print(" , Cb1= " + String(cb1));
    Serial.print(" , Est_enc1= " + String(est_enc1));
    Serial.print(" , pas_tota1= " + String(pas_tota1));
    Serial.print("\t");
    Serial.print("Ca2= " + String(ca2));
    Serial.print(" , Cb2= " + String(cb2));
    Serial.print(" , Est_enc2= " + String(est_enc2));
    Serial.print(" , pas_tota2= " + String(pas_tota2));
    Serial.println();
    delay(30);  // tiempo minimo que toca esperar para saturar el buffer de salida, minimo -> 9  
}
