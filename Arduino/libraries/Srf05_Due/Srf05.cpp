/*
  Srf05.cpp - Library for flashing Srf05 code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Srf05.h"

Srf05::Srf05(int pin){
  u1 = pin;
}

void Srf05::Read_Cent(){
  detachInterrupt(u1);
  d1 = 0;di1 = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(u1, OUTPUT);
  digitalWrite(u1, HIGH);
  delayMicroseconds(10);
  digitalWrite(u1, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(u1, INPUT);
  mu1 = false;
  //attachInterrupt(u1, * void, RISING);
  mu1 = true;
}

void Srf05::Read_Time(){
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(250);
}

void Srf05::fu1(){
 /*if (mu1){
    if (d1 == 0){
      d1 = micros();
      attachInterrupt(u1, Srf05::fu1, FALLING);
    } 
    else{
      d1 = micros()-d1; 
      di1 = d1/58.30;
    } 
  }*/
}

