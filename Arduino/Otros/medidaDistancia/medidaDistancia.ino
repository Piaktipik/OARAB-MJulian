
const int EPin = 2;
const int TPin = 3;

long duration, cm, da;
const byte disMax = 50;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(TPin, OUTPUT);
  pinMode(EPin, INPUT);
}

void loop() {
  // Variables para conversion del tiempo del pulso a cm
  

  // Emitimos Señal disparo
  digitalWrite(TPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TPin, LOW);

  // Capturamos el tiempo tomado por el pulso
  duration = pulseIn(EPin, HIGH, 50000); // tiempo maximo de 50ms

  // Convertimos tiempo a cm
  cm = microsecondsToCentimeters(duration);
  if (cm > disMax) {
    cm = disMax;
  }
  // promedio datos
  if (cm != disMax && cm != 0){
    da = cm;
  }


  
  Serial.println(da);
  //Serial.print("cm");
  //Serial.println();

  delay(30);
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 58.30;
}
