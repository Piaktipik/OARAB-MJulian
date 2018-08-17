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
#include<DSM501.h>

#define DSM501_PM10 3 // v2 1um
#define DSM501_PM25 4 // v1 1um

DSM501 dsm501(DSM501_PM10, DSM501_PM25);

unsigned long tarranque = 0;

//////////////////////////////////////////////////-
void setup() {
  Serial.begin(9600);
  dht.begin();
  // Initialize DSM501
  dsm501.begin(MIN_WIN_SPAN);
  // tiempo arranque
  tarranque = millis();
}
//////////////////////////////////////////////////-
void loop() {
  if (tarranque + 60000 < millis()) {
    // call dsm501 to handle updates.
    dsm501.update();

    // get PM density of particles over 1.0 μm
    Serial.print("PM10: ");
    Serial.print(dsm501.getParticalWeight(0));
    Serial.println(" ug/m3");

    // get PM density of particles over 2.5 μm
    Serial.print("PM25: ");
    Serial.print(dsm501.getParticalWeight(1));
    Serial.println(" ug/m3");

    //Serial.print("AQI: ");
    //Serial.println(dsm501.getAQI());

    // get PM2.5 density of particles between 1.0~2.5 μm
    Serial.print("PM2.5: ");
    Serial.print(dsm501.getPM25());
    Serial.println(" ug/m3");
    //
    Serial.println("//////////////////");
  }



  
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
