// Codigo por: Julian Galvez Serna
/////// Definiciones compilador
#define noSD 0


// librerias
#include <Wire.h>
#include <L3G4200D.h>
#include <LSM303.h>
#include <SPI.h>

#if noSD
#include <SD.h>
const int chipSelect = 10;
#endif

L3G4200D gyro;
LSM303 compass;

char report[80];
unsigned long tiempo = 0;

bool mostrarSerial = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  gyro.enableDefault();
  compass.init();
  compass.enableDefault();


#if noSD
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  } else {
    Serial.println("card initialized.");
  }
#endif

}

void loop() {

  while (millis() >= tiempo + 50) {
    tiempo = millis();
    int sensorValue = analogRead(A1);
    int corriente = sensorValue;

    gyro.read();
    compass.read();

    String datos = "";
    datos += "T";
    datos += millis();
    datos += "A";
    datos += compass.a.x;
    datos += "B";
    datos += compass.a.y;
    datos += "C";
    datos += compass.a.z;
    datos += "M";
    datos += compass.m.x;
    datos += "N";
    datos += compass.m.y;
    datos += "O";
    datos += compass.m.z;
    datos += "G";
    datos += gyro.g.x;
    datos += "H";
    datos += gyro.g.y;
    datos += "I";
    datos += gyro.g.z;
    datos += "D";
    datos += corriente;
    Serial.println(datos);

#if noSD
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(datos);
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
#endif

  }

}
