/*Sensor de Temperatura y Humedad DHT11<br>Instrucciones:
  Recuerda descargar la libreria DHT para poder utilizar este sensor
  Conectaremos el Sensor DHT11 a 5v y el pin de señal a la entrada digital 7
*/
//////////////////////////////////////////////////- h y t
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//////////////////////////////////////////////////- co
int sensorValue;

//////////////////////////////////////////////////- ppm

//////////////////////////////////////////////////-
void setup() {
  Serial.begin(9600);
  dht.begin();
}
//////////////////////////////////////////////////-
void loop() {
  int h = dht.readHumidity();// Lee la humedad
  int t = dht.readTemperature(); //Lee la temperatura
  //////////////////////////////////////////////////-Humedad
  Serial.print("HR: ");
  Serial.print(h);//Escribe la humedad
  Serial.print("%");
  //delay (500);
  ///////////////////////////////////////////////////-Temperatura
  Serial.print(", T: ");
  Serial.print(t);//Escribe la temperatura
  Serial.print("C");
  ///////////////////////////////////////////////////-COncentracion CO2
  sensorValue = analogRead(1);       // read analog input pin 0
  Serial.print(", CO: ");
  Serial.println(sensorValue, DEC);  // prints the value read
  ///////////////////////////////////////////////////
  //delay (1000);
  ///////////////////////////////////////////////////
}
