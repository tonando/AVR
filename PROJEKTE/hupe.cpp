
#include "Macros.h"
#include "sintable.h"

volatile uint f = 100;
volatile uint p = 50;

int main(void){

	uchar cnt=0;
	
	BUTTON(3);
	BUTTON(4);
	OUTPUT(2);
	
	// TCCR1 = B(CTC1) | 3;
	TCCR0A = B(WGM01);
	TCCR0B = 5;
	// OCR1A = 40;
	OCR0A = 33;
	// TCNT1=TCNT0=0;
	TIMSK |= /* B(OCIE1A) | */ B(OCIE0A);
	
	sei();

	while(1){
		if(!READ(4))
			TCCR0B = 5;
		else
			TCCR0B = 7;
			
		if(!READ(3)){
			// ulong df = 1000000UL / (f);
			// ulong don = df * (float)(p / 100.0);
			// ulong doff = df - don;
			
			// ulong don = MAX(600,df/2);
			// ulong doff = don;
			
			PORTB ^= B(2);
			delay_us(f);
		}
		
		// if(cnt == (SINSIZE*4)) cnt = 0;
		cnt++;
		
		if(cnt&3==0) OCR0A = 60 + (cnt>>3);
	}

	return 0;
}


// ISR(TIM1_COMPA_vect){
	// outs[0] = !outs[0];
// }
ISR(TIM0_COMPA_vect){
	f= 920 + (SINTABLE[p&(SINSIZE-1)]<<1);
	p++;
	p &=0x4ff;
	// if(p>=SINSIZE*4) p =0;
}

