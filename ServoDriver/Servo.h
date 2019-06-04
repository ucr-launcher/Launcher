
#ifndef _SERVO_H
#define _SERVO_H

#include "mbed.h"

class Servo
{
public:
   Servo(PinName pin);
   ~Servo(void);

   void write(unsigned char degree);

private:
   void convert(unsigned char degree);

   PwmOut _servo;
   unsigned int pulse;
};

#endif
           
