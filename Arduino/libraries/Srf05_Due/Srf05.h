/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Srf05_h
#define Srf05_h

#include "Arduino.h"

class Srf05
{
  public:
    Srf05(int pin);
    void Read_Cent();
    void Read_Time();
    int di1;
  private:
    byte u1;
    unsigned long d1;
    bool mu1 = false;
    void fu1();
};

#endif

