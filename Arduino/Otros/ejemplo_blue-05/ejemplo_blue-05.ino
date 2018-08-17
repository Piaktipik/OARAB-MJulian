// Basic Bluetooth sketch HC-05_AT_MODE_01
// Communicate with a HC-05 using the serial monitor
//
// The HC-05 defaults to communication mode when first powered on you will need to manually enter AT mode
// The default baud rate for AT mode is 38400
// See www.martyncurrey.com for details
//
 
 
//#include <SoftwareSerial.h>
//SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// 
 
char caracter = ' ';
 
void setup(){
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
 
    // HC-05 default serial speed for AT mode is 38400
    //BTserial.begin(38400);  
}
 
void loop(){
 
//    // Keep reading from HC-05 and send to Arduino Serial Monitor
//    if (BTserial.available())
//    {  
//        c = BTserial.read();
//        Serial.write(c);
//    }
// 
//    // Keep reading from Arduino Serial Monitor and send to HC-05
//    if (Serial.available())
//    {
//        c =  Serial.read();
//        BTserial.write(c);  
//    }
 
}
