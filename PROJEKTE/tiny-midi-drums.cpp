
#include <Macros.h>
#define TX_PIN 1
#include "tiny_midi.h"

#define TRESH 	6
#define NCHANS 	3
#define DECAY	60

const uchar chans[NCHANS] = {3,2,1};
uchar ain[NCHANS];
uchar current;

volatile uchar on;
volatile uchar timeout[NCHANS];

int main (void)
{ 
	BUTTON(0);
	INPUT(3);
	INPUT(4);
	INPUT(2);
	
	TCCR1 = 6;
	// TCNT1=0;
	TIMSK |= B(TOIE1);

	tiny_serial_init();	// 9600
	chan=3;
	
	sei();

	while(1){
		
		uchar a = adc_read(chans[current]);
		uchar n = 40 + (current);
		
		// shift
		if(!READ(0)) 
			n += NCHANS;

		//change
		if( a > (ain[current] + TRESH)/*  && a > TRESH  */&& timeout[current] < (255-DECAY)){
			uchar vol = MIN(127,a*2);
			// ain[current][1]=0;
			
			// if(vol > 10){
				if(rbi(on,current))
					note_off(n);
					
				note_on(n,vol);
				timeout[current] = 255;
				sbi(on,current);
				// _delay_ms(11);
				// note_off(38);
			// }
		}
		
		// ain[current][1] = MAX(ain[current][1],a);
		ain[current] = a;	
	
		// if(ain[current][1] < ain[current][0])
			// }
			if(!timeout[current] && rbi(on,current)){
				cbi(on,current);
				note_off(n);
				ain[current] = 0;
			}
			
		current++;
		if(current == NCHANS) current=0;
		// current &= 3;
	}
	
	return 0;
}

ISR(TIMER1_OVF_vect){
	uchar i=NCHANS;
	
	do{i--;
		if(timeout[i])
			timeout[i]--;

		
	}while(i);
}
