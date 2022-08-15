#include "Macros.h"
#include "Anal.h"
#include "math.h"

#include <util/delay.h>

#define STEPS 3
#define THRES 10
#define DEL_US 512
#define BYTE_PI 0.025

int spd;
uchar M[2] = {100,200};
volatile uchar  pwm_counter;
uchar counter;

#define aRead() analRead(2)

// #define pin_on(p) (PORTB |= _BV((p)))
// #define pin_off(p) (PORTB &= ~_BV((p)))
// #define pin_read(p) ((PINB >> (p)) & 1)


bool getM(uchar m){
	return (M[m] >= pwm_counter);
}
int main(void){ 

   DDRB = 3;
   PORTB &= ~3;
      TCCR0B = 4;
	  TIMSK0 |= _BV(TOV0);
	  sei();
   while(1)
    { 

	 
    spd = aRead();

		int i=spd>>2;
		do{	--i;
		// _delay_us(DEL_US); 

			_delay_ms(1); 
		}while(i);
		
		counter++;
		counter&=255;
		
//		double s = sin(BYTE_PI * counter) * 127 + 127;
		M[0] = (uchar)counter;
		// M[0] = (uchar)s;
	}
	return 0;
 }
 
 ISR(TIM0_OVF_vect){
	pwm_counter++;
	pwm_counter &= 255;
	
	if(getM(0))
		PORTB |= 1;
	else
		PORTB &= ~1;
 }
  