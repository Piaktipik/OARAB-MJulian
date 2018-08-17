/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
const int valorMedio = 529;
const float Escala = 1024-valorMedio;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // convertimos el valor a corriente
  float medicionCorriente =   float(sensorValue-valorMedio)/Escala*12.5;
  // print out the value you read:
  Serial.print(sensorValue);
  Serial.print("\t");
  Serial.println(medicionCorriente);
  delay(1);        // delay in between reads for stability
}
