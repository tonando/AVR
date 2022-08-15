
#include <Macros.h>
#include <util/delay.h>
// #include <math.h>

#define MOSI 	1
#define LATCH 	3
#define CLOCK 	2
#define BUTTON 	0

 // #define PWM_DIV 4
// #include <shiftOut.h> 
#include <Anal.h>

#define LEDS 8
//#include "IMAGES.h"
uchar leds[LEDS];
#define SINSIZE 64
#include "sintable.h"
// const unsigned char SINTABLE[32] = {8, 9, 11, 12, 13, 14, 15, 15, 16, 15, 15, 14, 13, 12, 11, 9, 7, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6};

uint a=200;
uchar output,cnt;
volatile uchar ovf;

int main(void){ 
	DDRB = _BV(CLOCK) | _BV(LATCH) | _BV(MOSI);
	PORTB =  _BV(BUTTON) | _BV(MOSI);

	TCCR0B = 1;
	TIMSK = _BV(TOIE0);

	sei();
	// analInit(2);
	//setupAdc();
	//  uchar cnt=0;
	uchar led_index=0;
	uint r=0;
   
	while (1){

   if(!cnt){
	   r=lfsr_rand();
	   leds[led_index] = (r>>8) & 0x0f;
	   led_index = r&7;
   }
   cnt++;
   cnt&=63;
      
   
	a = analRead(3);
	// output = _BV((SINTABLE[cnt]>>1));
	if(PINB&1){
		leds[led_index] = SINTABLE[cnt]>>4;
		do{ a--;
		_delay_ms(1);
		}while(a);
   }else{
	   
		leds[led_index] = SINTABLE[cnt]>>4;
		_delay_ms(10);
		// for(uchar i=0;i<8;i++)
			// leds[i] = lfsr_rand()&0x0f;
   }
   if(a>100)
	   leds[3] = 0xff;
	// output = _BV(cnt>>2);//IMAGE[cnt&7][cnt>>1];
   
   // output ^= 0xff;
   
	
	}

	return 0;
}
	
ISR(TIMER0_OVF_vect){
	//  if(output > cnt)ON(MOSI);else OFF(MOSI);return;
	ovf++;
	uchar b = ovf&7;
	
	OFF(MOSI);
	OFF(CLOCK);
	
	if(!b){
		// do{
			OFF(LATCH);
			ON(LATCH);
		// }while(0);
	}
	

	if((leds[b] < ovf>>4))// && rbi(output,b) == 1)	
		ON(MOSI);

	ON(CLOCK);

	// OFF(MOSI);
	// OFF(CLOCK);	
}