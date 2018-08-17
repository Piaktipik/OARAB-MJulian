
// Variables medicion tiempos Motor 1
unsigned long ultimotpaM1 = 0;
unsigned long ultimotpbM1 = 0;
unsigned long ultimopaM1 = 0;
unsigned long ultimopbM1 = 0;

// Variables medicion tiempos Motor 2
unsigned long ultimotpaM2 = 0;
unsigned long ultimotpbM2 = 0;
unsigned long ultimopaM2 = 0;
unsigned long ultimopbM2 = 0;


int inline pro_est_AM1(int pA, int pB, int estado) {
  int x = 0;
  switch (estado) {
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
  est_encM1 = x;
  //esp_pro_estM1(pA, pB, x);
}

void inline pro_est_BM1(int pA, int pB, int estado) {
  int x = 0;
  switch (estado) {
    case 1:
      x = 4;
      pas_totM1--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_totM1++;
      break;
  }
  est_encM1 = x;
  //esp_pro_estM1(pA, pB, x);
}


void inline ENCODERAM1() {
  // obtengo el delta de tiempo entre flancos
  //ultimopaM1 = ultimotpaM1 - micros();
  //ultimotpaM1 = micros();

  pro_est_AM1(pinAM1, pinBM1, est_encM1);
  // aumento el contador de pasos
  caM1++;
}

void inline ENCODERBM1() {
  // obtengo el delta de tiempo entre flancos
  //ultimopbM1 = ultimotpbM1 - micros();
  //ultimotpbM1 = micros();

  pro_est_BM1(pinAM1, pinBM1, est_encM1);
  // aumento el contador de pasos
  cbM1++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int inline obt_est(int pA, int pB) {
  int x = digitalRead(pA) * 2 + digitalRead(pB);
  switch (x) {
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

void activarEncoders(int pAM1, int pBM1, int pAM2, int pBM2) {
  // se ponen como entradas los puertos de los encoderes
  pinMode(pAM1, INPUT);
  pinMode(pBM1, INPUT);
  pinMode(pAM2, INPUT);
  pinMode(pBM2, INPUT);

  // se incializan las bases de tiempo
  ultimotpaM1 = 0;
  ultimotpbM1 = 0;
  ultimopaM1 = 0;
  ultimopbM1 = 0;

  ultimotpaM2 = 0;
  ultimotpbM2 = 0;
  ultimopaM2 = 0;
  ultimopbM2 = 0;

  // se verifica el estado
  est_encM1 = obt_est(pAM1, pBM1);

  attachInterrupt(digitalPinToInterrupt(pAM1), ENCODERAM1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pBM1), ENCODERBM1, CHANGE);
      
  //esp_pro_estM1(pAM1, pBM1, est_encM1);

  //est_encM2 = obt_est(pAM2, pBM2);
  //esp_pro_estM2(pAM2, pBM2, est_encM2);
}

// encoder motor 1

/*
void esp_pro_estM1(int pA, int pB, int est_encM1) {
  switch (est_encM1) {
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM1, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM1, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM1, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM1, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM1, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM1, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM1, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM1, FALLING);
      break;
  }
}
*/

/*
// encoder motor 2

void ENCODERAM2() {
  // obtengo el delta de tiempo entre flancos
  ultimopaM2 = ultimotpaM2 - micros();
  ultimotpaM2 = micros();

  pro_est_AM2(pinAM2, pinBM2, est_encM2);
  // aumento el contador de pasos
  caM2++;
}

void ENCODERBM2() {
  // obtengo el delta de tiempo entre flancos
  ultimopbM2 = ultimotpbM2 - micros();
  ultimotpbM2 = micros();

  pro_est_BM2(pinAM2, pinBM2, est_encM2);
  // aumento el contador de pasos
  cbM2++;
}

void esp_pro_estM2(int pA, int pB, int est_encM2) {
  switch (est_encM2) {
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM2, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM2, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM2, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM2, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM2, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM2, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERAM2, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERBM2, FALLING);
      break;
  }
}

int pro_est_AM2(int pA, int pB, int estado) {
  int x = 0;
  switch (estado) {
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
  est_encM2 = x;
  esp_pro_estM2(pA, pB, x);
}

void pro_est_BM2(int pA, int pB, int estado) {
  int x = 0;
  switch (estado) {
    case 1:
      x = 4;
      pas_totM2--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_totM2++;
      break;
  }
  est_encM2 = x;
  esp_pro_estM2(pA, pB, x);
}
*/
