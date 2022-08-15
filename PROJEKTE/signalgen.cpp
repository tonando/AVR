#include "Macros.h"
#include "Anal.h"
//#include "Timers.h"
#include "util/delay.h"

#define SINSIZE 256
#include "sintable.h"

uchar mode;
uint speed=5;

int main(void){ 
	uchar cnt=0;
	bool dir=0;
	DDRB  = 0x07; //LED und PWM sind Ausgänge


	//FastPWM, TOP=0xFF, non-invert OC0x
	TCCR0A = (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
	TCNT0  = 0;
	OCR0A  = 0;  //50%
	OCR0B  = 0;  //50%
	TCCR0B = 1;//(1<<CS02);//fclk/256/8=4kHz PWM-Takt (+Start)
	sei();
    
   while(1){ 
	cnt++;
	cnt &= SINSIZE-1;
	if(!cnt){ 
		PORTB ^= _BV(PB2);
	}
	OCR0A = SINTABLE[cnt];

	OCR0B = cnt;
//	speed = analRead(3);
	
	for(uchar f=0; f<=speed; f++)
		_delay_us(100);
	
//	if(dir)
//	OCR0A = SINTABLE[SINSIZE-cnt-1];
//	else
continue;
	OCR0A = SINTABLE[cnt];
	
	
	if(dir){
	OCR0B++;
	if(OCR0B==255) dir = !dir;
	}
	else{
	OCR0B--;
	if(!OCR0B) dir = !dir;
	}
    }
 }


