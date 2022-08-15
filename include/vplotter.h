#ifndef VPLOTTER_H
#define VPLOTTER_H

#include <Macros.h>

// cart
float cPosX,cPosY;
float maxPosX,maxPosY;

// polar
ulong lMotA,lMotB; // schnur 

// motors
uchar motorA,motorB;

uchar get_step(uchar);
uchar step(uchar,bool);

// cal
void set_center();

#endif
