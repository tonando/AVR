#include "Macros.h"

// #include "math.h"


#include <util/delay.h>

#define STEPS 7
#define THRES 10
#define DEL_US 512
//const uchar STEP_SEQUENCE[STEPS] = {0b1010,0b10000010,0b0110,0b10000100,0b0101,0b10000001,0b1001,0b10001000};
const uchar STEP_SEQUENCE[STEPS+1] = {0b10101010,0b00100010,0b01100110,0b01000100,0b01010101,0b00010001,0b10011001,0b10001000};
// const uchar STEP_SEQUENCE[STEPS+1] = {0b10101010,0b01100110,0b01010101,0b10011001};

//uchar pins[4] = {0,1,2,3};
uchar stepper;
// char dir;
// signed int a,spd;
volatile bool pval;

// #define pin_on(p) (PORTB |= _BV((p)))
// #define pin_off(p) (PORTB &= ~_BV((p)))
// #define pin_read(p) ((PINB >> (p)) & 1)



void step(){
 //  if(S_MODE[i] == 0 && (C_STEP[i] & 1) == 1) errors++;
   //idle_timer = TIMER;
   
   // if(dir){
	   //C_STEP[i] = (C_STEP[i] + S_MODE[i] + 1) & 7;
	 stepper++;
	   stepper    &= STEPS;
   // }
   // else{
//	   if(C_STEP[i] && C_STEP[i] < 8)
	  
	 // if(stepper)
	    // stepper--;
	 // else
	  // stepper = STEPS;
   // }
   //shift(STEP_SEQUENCE[stepper]);
   PORTB = (STEP_SEQUENCE[stepper] & 0x0f) | 0xf0;

   
} 
int main(void){ 

   DDRB = 0x0f;
PORTB = _BV(PB4);
pval=true;
   
   // uchar last_val=1;
    // PORTB |= _BV(PB1);            //internen Pull-Up-Widerstand an PB1 einschalten
    PCMSK |= _BV(PCINT4);       //PCMSK – Pin Change Mask Register
    // GIMSK  |= (1<<INT0);        //External Interrupt Request 0 Enable
    GIMSK  |= _BV(PCIE);        //External Interrupt Request 0 Enable
    // MCUCR |=(0<<ISC01) | (0<<ISC00);             // Interrupt auslösen wenn INT0 = GND
    sei();  

          
   while(1)
    { 
/* 	uchar p = rbi(PINB,4);
	if(p != last_val){
		if(!p)
			step(); 
		else
			    PORTB = _BV(PB4);
		last_val = p;
	} */
 }
 }
 
 ISR (PCINT0_vect) {
	pval = !pval;
	if(pval)
		PORTB = _BV(PB4);
	else
		step(); 
    }