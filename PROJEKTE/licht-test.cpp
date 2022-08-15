#include "Macros.h"

uint speed=2;
uint str=160;
uint len=100;
uchar out = 0;
volatile uchar ovf = 0;

uchar adc(uchar c)
{
    ADMUX |= c;
    // Start the conversion
    ADCSRA |= (1 << ADSC);
 
    // Wait for it to finish
    while (ADCSRA & (1 << ADSC));
    return (ADCH & 0xff);
 
}

int main(void){
	
		
	OUTPUT(1);
	ON(1);
	OUTPUT(0);
	INPUT(2);
	ON(2);
	INPUT(4);
	INPUT(3);
	
    ADMUX = (1 << ADLAR);
    ADMUX |= 3;
    ADCSRA |= (1 << ADEN);
	
	TCCR0B = READ(2) ? 1 : 3;
	TIMSK = B(TOIE0);
	OFF(2);
	
	sei();
	uchar cnt=0;

	while(1){
		cnt++;
		cnt&=15;
		
			delay_ms((10+speed));
		
		if((cnt&7)==4)
			out=0;
		if(cnt&1){
				len = adc(2)>>4;
				str = adc(1)&255;
			speed = adc(3)>>1;
		}	
		if(cnt >= len)
			out = 0;
		else
			out = str;

		if(!cnt)
			fbi(PORTB,1);
	
	}
	return 0;
}

ISR(TIMER0_OVF_vect){
	if(out > ovf)
		ON(0);
	else
		OFF(0);
	
	ovf++;
	
}
