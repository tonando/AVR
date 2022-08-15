#ifndef SEVEN_h 
#define SEVEN_h 

#include "Macros.h"

#define INVERT 		1
#define USE_LATCH 	1
#define BLINK_DELAY 50

#define SHIFT_OUT 		PB0
#define SHIFT_CLOCK  	PB2
#define SHIFT_LATCH 	PB1

#define INIT_SHIFT (PORTB |= _BV(SHIFT_OUT) | _BV(SHIFT_CLOCK | _BV(SHIFT_LATCH) )

#define S_A		0b11101110 // A
#define S_B     0b00111110 // b
#define S_C     0b10011100 // C
#define S_D     0b01111010 // d
#define S_E     0b10011110 // E
#define S_F     0b10001110 // F
#define S_H     0b01101110 
#define S_I     0b01100000 

uint8_t S_DIGITS[10] = {
   0b11111100, // 0
   0b01100000, // 1
   0b11011010, // 2
   0b11110010, // 3
   0b01100110, // 4
   0b10110110, // 5
   0b10111110, // 6
   0b11100000, // 7
   0b11111110, // 8
   0b11110110  // 9
};

static void shiftOut(uint8_t a, uint8_t b){

   byte s=2;
    do{s--;
       
       uint8_t i=8;
      do{i--;
	     OFF(SHIFT_CLOCK); 
#if INVERT
	if(!rbi(s?a:b,7-i))  
#else
	if(rbi(s?a:b,7-i))
#endif   	 
		   ON(SHIFT_OUT);
	     else
		   OFF(SHIFT_OUT);
	 
	     ON(SHIFT_CLOCK);  
    }while(i);	
   }while(s);
#if USE_LATCH
OFF(SHIFT_LATCH); 
OFF(SHIFT_CLOCK); 
ON(SHIFT_LATCH); 
#endif   
}
// TODO
void blink(byte t){
      REPEAT(t){

      shiftOut(0,0);
      _delay_ms(BLINK_DELAY);
	  //shiftOut(a,b);
      _delay_ms(BLINK_DELAY);
      }
}
void seven_out(uint o){

   uint8_t d1=0,d2=0;
   
   if(o >= 1000){
      d2=1;
      a = (o / 1000) % 10;
      b = (o / 100) % 10;
   }else{
      d1=1;
      a=(o / 100) % 10;
      b=(o / 10) % 10;
   }
   
   a = S_DIGITS[a] | d1;
   b = S_DIGITS[b] | d2;
//   shiftOut(a,b);
} 

#endif