#include <Wire.h>
#include <L3G4200D.h>
#include <LSM303.h>
#include <SPI.h>
#include <SD.h>

L3G4200D gyro;
LSM303 compass;

char report[80];
unsigned long tiempo = 0;

bool mostrarSerial = true;

const int chipSelect = 10;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  gyro.enableDefault();
  compass.init();
  compass.enableDefault();

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
  
  while(millis()>=tiempo+100){
    tiempo = millis();
    int sensorValue = analogRead(A1);
    int corriente = sensorValue;

    gyro.read();
    compass.read();
  
    if(mostrarSerial){
      snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d  G: %6d %6d %6d",
        compass.a.x, compass.a.y, compass.a.z,
        compass.m.x, compass.m.y, compass.m.z,
        (int)gyro.g.x, (int)gyro.g.y, (int)gyro.g.z);
      Serial.print(report);
    }
    
    String datos = "";
    datos += "T";
    datos += millis();
    datos += "AX";
    datos += compass.a.x;
    datos += "Y";
    datos += compass.a.y; 
    datos += "Z";
    datos += compass.a.z;
    datos += "MX";
    datos += compass.m.x; 
    datos += "Y";
    datos += compass.m.y; 
    datos += "Z";
    datos += compass.m.z;
    datos += "GX";
    datos += (int)gyro.g.x; 
    datos += "Y";
    datos += (int)gyro.g.y; 
    datos += "Z";
    datos += (int)gyro.g.z;
    datos += "C";
    datos += corriente;
    
    if(!mostrarSerial){
      Serial.println(datos);
    }

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
    
  }
}
