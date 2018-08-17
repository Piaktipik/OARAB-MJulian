
/*
  VEML6070-Breakout (Digital UV Light Sensor).
  Rset=270k on breakout, UVA sensitivity: 5.625 uW/cm�/step
*/

// librerias
#include <SD.h> // libreria para el manejo de SD

#include "DHT22.h"
// Only used for sprintf
#include <stdio.h>

#include <Wire.h>

#include "gps.h"

// variables

//////////////////////////// SD
bool sd_ok = false;
File dataFile;
const int chipSelect = 10;
String datos = "";
int sensor = 0;


// SHT22
// Data wire is plugged into port 9 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 9

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);
int humedad = 0;
int temperatura = 0;

// Ultrasonido
const int pingPin = 4; // Eco
const int Pin = 3; // trigger
long cm = 0;

// Sensor UV
#define I2C_ADDR 0x38 //0x38 and 0x39
//Integration Time
#define IT_1_2 0x0 //1/2T
#define IT_1   0x1 //1T
#define IT_2   0x2 //2T
#define IT_4   0x3 //4T
unsigned int UV = 0;

//////////////////////////// GPS
#define gpsRESET_PIN A2
// Module constants
static const uint32_t VALID_POS_TIMEOUT = 2000;  // ms

// pin idicador led ok
#define ledOK 5

void setup() {
  Serial.begin(9600);  // activo el puerto serial para enviar datos

  // initialize serial communication:
  pinMode(Pin, OUTPUT);
  pinMode(pingPin, INPUT);

  //-------------------------conecto con la SD-------------------------
  if (SD.begin(chipSelect)) {
    sd_ok = true;
  }

  // puerto I2C Sensor UV
  Wire.begin();
  Wire.beginTransmission(I2C_ADDR);
  Wire.write((IT_1 << 2) | 0x02);
  Wire.endTransmission();

  pinMode(ledOK, OUTPUT);
  digitalWrite(ledOK, HIGH);
  delay(2000);
  digitalWrite(ledOK, LOW);

  ///////////////////// Setup GPS
  Serial.println("Setup Gps...");
  gps_setup();
  pinMode(gpsRESET_PIN, OUTPUT);
  digitalWrite(gpsRESET_PIN, LOW);
  delay(50);
  pinMode(gpsRESET_PIN, INPUT);
}

void loop() {
  // leo sensor de humedad y temperatura
  DHT22_Read();
  // leo sensor ultrasonido
  medir_distancia();
  // leo sensor UV
  leer_UV();

  // imprimimos datos
  datos = "";
  datos += String(gps_time);
  datos += ",";
  datos += String(millis());
  datos += ",";
  datos += String(cm);
  datos += ",";
  datos += String(UV);
  datos += ",";
  datos += String(humedad);
  datos += ",";
  datos += String(temperatura);
  //Serial.println(datos);                // imprimo por serial
  //datos = "";//*/

  // guardo en uSD
  guardarDatos();

  // Dormir X segundos
  //delay(1500);
  read_gps();
  /*Serial.print("Datos GPS       : Tiemp: ");
    Serial.print(gps_time);
    Serial.print("\t Lat: ");
    Serial.print(gps_lat * 1000);
    Serial.print("\t Lon: ");
    Serial.print(gps_lon * 1000);
    Serial.print("\t Alt: ");
    Serial.print(gps_altitude);
    Serial.print("\t Curso: ");
    Serial.print(gps_course);
    Serial.print("\t Fecha: ");
    Serial.println(gps_date);*/
}

// fin loop

void iniciarSd() {
  pinMode(chipSelect, OUTPUT);
  Serial.print(" ini.. ");
  if (SD.begin(chipSelect)) {
    Serial.println("OK ");
    sd_ok = true;

  } else {
    Serial.println("Err ");
    software_Reset();
  }
}

void guardarDatos() {
  //---------------------------cargo los datos-------------------------
  /*datos = "";
    //datos += String(millis);
    //datos += ",";
    datos += String(cm);
    datos += ",";
    datos += String(UV);
    datos += ",";
    datos += String(humedad);
    datos += ",";
    datos += String(temperatura);*/

  //----------------preparo el archivo para los datos-------------------
  String nombre = "d";
  nombre += String(gps_date);
  nombre += ".txt";
  if (sd_ok) {
    // se guarda en la SD
    dataFile = SD.open(nombre, FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      if (gps_altitude > 0) {
        Serial.print("OK ");
        digitalWrite(ledOK, HIGH);
      }
      dataFile.println(datos);
      dataFile.flush();
      dataFile.close();
      // led indicador guardado en SD

      // print to the serial port too:
      //Serial.print(datos);
      Serial.print("On: ");
      Serial.print(nombre);
      Serial.print(": ");
      Serial.println(datos);
      //pitar(50);   //Pitido indicando que se guardo correctamente la informacion
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.print("error opening:");
      Serial.println(nombre);
      dataFile.close();
      sd_ok =  false;
      software_Reset();
    }
    digitalWrite(ledOK, LOW);   // Apagamos led luego de guardar datos
  } else {
    iniciarSd(); // si sd no esta ok (sd_ok) tratamos de iniciarla nuevamente
  }
}

void DHT22_Read() {

  DHT22_ERROR_t errorCode;

  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  delay(500);

  //Serial.print("Requesting data...");
  errorCode = myDHT22.readData();
  switch (errorCode)
  {
    case DHT_ERROR_NONE:
      //Serial.print("Got Data ");
      temperatura = myDHT22.getTemperatureC();
      humedad = myDHT22.getHumidity();
      //      Serial.print(myDHT22.getTemperatureC());
      //      Serial.print("C ");
      //      Serial.print(myDHT22.getHumidity());
      //      Serial.println("%");
      // Alternately, with integer formatting which is clumsier but more compact to store and
      // can be compared reliably for equality:
      //
      //      char buf[128];
      //      sprintf(buf, "Integer-only reading: Temperature %hi.%01hi C, Humidity %i.%01i %% RH",
      //                   myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
      //                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
      //      Serial.println(buf);
      break;
    case DHT_ERROR_CHECKSUM:
      Serial.print("check sum error ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      break;
    case DHT_BUS_HUNG:
      Serial.println("BUS Hung ");
      break;
    case DHT_ERROR_NOT_PRESENT:
      Serial.println("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      Serial.println("ACK time out ");
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      Serial.println("Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      Serial.println("Data Timeout ");
      break;
    case DHT_ERROR_TOOQUICK:
      Serial.println("Polled to quick ");
      break;
  }
}

void medir_distancia() {
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();

  delay(100);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 58.30;
}

void leer_UV() {
  byte msb = 0, lsb = 0;
  //uint16_t uv;

  Wire.requestFrom(I2C_ADDR + 1, 1); //MSB
  delay(1);
  if (Wire.available())
    msb = Wire.read();

  Wire.requestFrom(I2C_ADDR + 0, 1); //LSB
  delay(1);
  if (Wire.available())
    lsb = Wire.read();

  UV = (msb << 8) | lsb;
  //Serial.println(UV, DEC); //output in steps (16bit)

  //delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void read_gps() {
  // Get a valid position from the GPS
  int valid_pos = 0;
  uint32_t timeout = millis();
  char lecturagps;
#ifdef DEBUG_GPS
  Serial.println("RGPS");
#endif

  gps_reset_parser();

  do {
    if (Serial.available()) {
      lecturagps = Serial.read();
#ifdef DEBUG_GPS
      Serial.print(lecturagps);
#endif
      valid_pos = gps_decode(lecturagps);
    }
  } while ( (millis() - timeout < VALID_POS_TIMEOUT) && ! valid_pos) ;

  if (valid_pos) {

  }
}

// Restarts program from beginning but does not reset the peripherals and registers
void software_Reset() {
  asm volatile ("  jmp 0");
}
