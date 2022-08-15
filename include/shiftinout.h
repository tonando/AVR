#ifndef SHIFTINOUT_h
#define SHIFTINOUT_h

#include "Macros.h"

//#define CLK PB2
//#define MOSI PB0
//#define MISO PB1
// #define LATCH PB4
// #define PS PB3

#define DDR DDRB
#define PORT PORTB
#define PIN PINB

#define pin_on(p) (PORT |= _BV((p)))
#define pin_off(p) (PORT &= ~_BV((p)))
#define pin_read(p) ((PIN >> (p)) & 1)

uchar clock_pin,miso_pin,mosi_pin,latch_pin;//,ps_pin;

void initShift(uchar _c,uchar _i, uchar _o, uchar _l){//, uchar _p){
	clock_pin=_c;
	miso_pin=_i;
	mosi_pin=_o;
	latch_pin=_l;
	// ps_pin=_p;
	//uint8_t old_ddrb = DDRB; // restor
//	DDRB = _BV(CLK) | _BV(MOSI) | _BV(LATCH) | _BV(PS);
	DDR |= _BV(clock_pin) | _BV(mosi_pin) | _BV(latch_pin) /* | _BV(ps_pin) */;
	DDR &= ~_BV(miso_pin);
	PORT |= _BV(latch_pin) /* | _BV(ps_pin) */; 
}

uint8_t shift(uint8_t d){
 
	// pin_off(ps_pin);
	pin_off(latch_pin);
	uint8_t _in = pin_read(miso_pin);
	
    uint8_t i=8;
    do{i--;
	  _in <<= 1;
      
	  pin_off(clock_pin); 
      
	  if(rbi(d,i))
		pin_on(mosi_pin);
	  else
		pin_off(mosi_pin);
      
	  if(pin_read(miso_pin))
		_in |= 1;
	  pin_on(clock_pin);  
   }while(i);	
	
	

	pin_on(latch_pin);
//	pin_on(ps_pin);
	
	pin_off(clock_pin);
	
	return _in;
}

#endif