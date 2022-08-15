#ifndef STEPPER_MOTOR_h
#define STEPPER_MOTOR_h

#include "Macros.h"

const uchar STEP_SEQUENCE[8] = {0b1010,0b0010,0b0110,0b0100,0b0101,0b0001,0b1001,0b1000};
uchar STEP_PINS[4] = {1,3,2,0};
uchar STEPS = 7;
bool fullstep = 0;      

uchar step(bool); 

uchar step(bool _dir){
	static uchar cstep = 0;
	
	if(_dir){
	 cstep++;
	 
	 if(cstep>STEPS)
		cstep = 0;
	}
	else{
	 if(cstep)
		cstep--;
	 else
		cstep = STEPS;
	}
   
	uchar _t = fullstep ? STEP_SEQUENCE[cstep&1] : STEP_SEQUENCE[cstep];

	uchar out = 0;
	for(uchar i=0;i<4;i++){
		if(rbi(_t,i)) sbi(out,STEP_PINS[i]);
	}
	PORTB &= 0xf0;	
	PORTB |= out;	
	
	return cstep;
} 

void stepper_init(uchar _a,uchar _b,uchar _c,uchar _d, bool _f){ 
	fullstep = _f;
	STEP_PINS[0] = _a;
	STEP_PINS[1] = _b;
	STEP_PINS[2] = _c;
	STEP_PINS[3] = _d;
	OUTPUT(STEP_PINS[0]);
	OUTPUT(STEP_PINS[1]);
	OUTPUT(STEP_PINS[2]);
	OUTPUT(STEP_PINS[3]);
}
void stepper_init(uchar _a,uchar _b,uchar _c,uchar _d) { stepper_init(_a,_b,_c,_d,false); }

void stepper_stop(){
	OFF(STEP_PINS[0]);
	OFF(STEP_PINS[1]);
	OFF(STEP_PINS[2]);
	OFF(STEP_PINS[3]);
}
#endif