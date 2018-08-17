
// ************************************** Encoder1 Motor Izquierdo - (Magnetico) ************************************** 
// -------------------- puertos usados del arduino
// ----- encoder 1 -----
int pinA1 = 22;
int pinB1 = 23;

// -------------------- Variables para la medicion
// ----- encoder 1 -----
volatile unsigned int ca1 = 0; volatile unsigned int cb1 = 0;    // contador de entrada a la interrupcion
volatile int est_enc1 = 0;                              // estado del encoder {1,2,3,4}
volatile long pas_tota1 = 0;                             // pasos absolutos del encoder

// -------------------- Variables para la medicion tiempo entre pulsos
volatile unsigned long tiempoDesdeAnteriorPulsoA1 = 0;
volatile unsigned long tiempoDesdeAnteriorPulsoB1 = 0;
volatile unsigned long deltaTiempoPulsoA1 = 0;
volatile unsigned long deltaTiempoPulsoB1 = 0;

// variables para ambos encoders
volatile bool calcularDeltaTiempoPulsos = false;

//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ encoders  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void activarEncoder1(int pA, int pB){
  // se ponen como entradas los puertos del encoder
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  // se incializan las bases de tiempo
  tiempoDesdeAnteriorPulsoA1 = 0;
  tiempoDesdeAnteriorPulsoB1 = 0;
  deltaTiempoPulsoA1 = 0;
  deltaTiempoPulsoB1 = 0;
  // se verifica el estado 
  est_enc1 = obt_est1(pA,pB);
  // 
  esp_pro_est1(pA, pB, est_enc1);
}

void ENCODERA1() {
  if(calcularDeltaTiempoPulsos){
    // obtengo el delta de tiempo entre flancos
    deltaTiempoPulsoA1 = abs(micros() - tiempoDesdeAnteriorPulsoA1);
    tiempoDesdeAnteriorPulsoA1 = micros();
  }
  pro_est_A1(pinA1,pinB1,est_enc1);
  // aumento el contador de pasos
  ca1++;
}

void ENCODERB1() {
  if(calcularDeltaTiempoPulsos){
    // obtengo el delta de tiempo entre flancos
    deltaTiempoPulsoB1 = abs(micros() - tiempoDesdeAnteriorPulsoB1);
    tiempoDesdeAnteriorPulsoB1 = micros();
  }
  pro_est_B1(pinA1,pinB1,est_enc1);
  // aumento el contador de pasos
  cb1++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int obt_est1(int pA,int pB){
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

void esp_pro_est1(int pA,int pB,int est_enc){
  switch(est_enc1){
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, FALLING);
      break;
  }
}

int pro_est_A1(int pA,int pB,int estado){
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
  est_enc1 = x;
  esp_pro_est1(pA,pB,x);
}

void pro_est_B1(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 4;
      pas_tota1--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_tota1++;
      break;
  }
  est_enc1 = x;
  esp_pro_est1(pA,pB,x);
}
