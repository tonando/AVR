#ifndef SHIFTOUT_h
#define SHIFTOUT_h

#include "Macros.h"

#define DDR DDRB
#define PORT PORTB
#define PIN PINB

#define pin_on(p) (PORT |= _BV((p)))
#define pin_off(p) (PORT &= ~_BV((p)))
#define pin_read(p) ((PIN >> (p)) & 1)

// bottleneck?
uchar clock_pin,miso_pin,mosi_pin,latch_pin;//,ps_pin;


void shiftOut(uchar d);
void shiftOut16(unsigned int d);
void shiftOut32(unsigned long d);
void initShift(uchar _c, uchar _o, uchar _l);
void shiftBegin();
void shiftEnd();
void shiftBit(bool b);



void initShift(uchar _c, uchar _o, uchar _l){//, uchar _p){
	clock_pin=_c;
	mosi_pin=_o;
	latch_pin=_l;

	DDR |= _BV(clock_pin) | _BV(mosi_pin) | _BV(latch_pin) /* | _BV(ps_pin) */;
	PORT |= _BV(latch_pin) /* | _BV(ps_pin) */; 
}
void shiftBegin(){ 
	pin_off(latch_pin);
	
}
void shiftEnd(){ 
	pin_on(latch_pin);
	pin_off(clock_pin);
}
void shiftBit(bool b){
	  pin_off(clock_pin); 
	  if(b)
		pin_on(mosi_pin);
	  else
		pin_off(mosi_pin);            
      
	  pin_on(clock_pin);  
}

void shiftOut(uchar d){
 
	shiftBegin();
	uchar i=8;
	do{i--;
		shiftBit(rbi(d,i));	
	}while(i);
	shiftEnd();

}
void shiftOut16(unsigned int d){
	shiftBegin();
	uchar i=16;
	do{i--;
		shiftBit(rbi(d,i));	
	}while(i);
	shiftEnd();
}
void shiftOut32(unsigned long d){
	shiftBegin();
	uchar i=32;
	do{i--;
		shiftBit(rbi(d,i));	
	}while(i);
	shiftEnd();
}

#endif