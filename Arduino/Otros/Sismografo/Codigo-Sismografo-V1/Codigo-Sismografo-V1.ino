// Codigo sismografo V1

#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File dataFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // dalcaramos todos los puertos como entradas
  for (int pin = 1; pin < 10; pin++) {
    pinMode(pin, INPUT);
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  // string para armar el dato a guardar
  String dataString = "";

  // leemos el pin analogo
  dataString += millis();
  dataString += ",";
  int sensor = analogRead(1);
  dataString += String(sensor);

  dataFile = SD.open("datosSis.txt", FILE_WRITE);
  // si el archivo esta abierto, escribimos:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // mostramos el dato guardado por serial
    Serial.println(sensor);

  } else {
    Serial.println("error opening datalog.txt");
  }
}









