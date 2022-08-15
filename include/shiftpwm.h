#ifndef SHIFTPWM_h
#define SHIFTPWM_h

#include "Macros.h"

// #include "Macros.h"

#ifndef NUM_REGS
	#define NUM_REGS 1
#endif
#ifndef PWM_DIV
	#define PWM_DIV 0
#endif

#define NUM_PINS (8*NUM_REGS)
#define MAX_VAL (255 >> PWM_DIV)

#define DDR DDRB
#define PORT PORTB
#define PIN PINB

#define pin_on(p) (PORT |= _BV((p)))
#define pin_off(p) (PORT &= ~_BV((p)))
#define pin_read(p) ((PIN >> (p)) & 1)

typedef unsigned char uchar;

uchar clock_pin,mosi_pin,latch_pin;//,ps_pin;

uchar PINS[NUM_PINS];
volatile uchar c_cycle;

bool setPin(uchar _p, uchar _v){

	// uchar pin = _p % REG_LENGTH;
	// uchar reg = (_p - pin) / REG_LENGTH;
	if(_p > NUM_PINS)  
		return false;
		
	#if PWM_DIV > 0
	 _v >>= PWM_DIV;
	#endif

	PINS[_p] = _v;
	// uchar pin = _p & 7;
	// uchar reg = _p >> 3; // div by 8
	
	// REGISTERS[reg].pins[pin] = _v;
	
	return true;
}

void initShiftPwm(uchar _c,uchar _o,uchar _l){
	clock_pin=_c;
	mosi_pin=_o;
	latch_pin=_l;
	
	DDR |= _BV(_c) | _BV(_o) | _BV(_l);
	PORT |= _BV(_l);
	
}

bool nextCycle(){
	if(c_cycle >= MAX_VAL)
		c_cycle = 0;
	else
		c_cycle++;

	pin_off(latch_pin);
	
	uchar i=NUM_PINS;
    do{i--;
      
	  pin_off(clock_pin); 
      
	  if(PINS[i] >= c_cycle)
		pin_on(mosi_pin);
	  else
		pin_off(mosi_pin);
      
	  pin_on(clock_pin);  
   }while(i);	
	
	pin_on(latch_pin);
	pin_off(clock_pin);
/* 	shiftBegin();
	
	uchar r = NUM_REGS;
	do{r--;
		REGISTERS[r].out = 0;
		uchar i = 8;
		do{i--;
			if(REGISTERS[r].pins[i] >= c_cycle)
				REGISTERS[r].out |= _BV(i);
		}while(i);
		
		shiftByte(REGISTERS[r].out);
	}while(r);
	
	shiftEnd();
 */	
	return c_cycle;
}
void fadeOut(){
      uchar p = NUM_PINS;
	    
      do{p--;
		 if(PINS[p])
			PINS[p]--;
		 else
			PINS[p]=MAX_VAL;
      }while(p);
}
#endif