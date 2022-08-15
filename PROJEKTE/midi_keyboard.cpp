// HEADER
#define BAUD 9600
#include "Macros.h"
// #include "Anal.h"
#include "shiftinout.h"
// #include "serial.h"
#include "midi.h"
#include "util/delay.h"

#define ROWS 4 // 8 = 500hz @ 16mhz
#define POLY 8

uchar last[8],in[8];
uchar poly;

int main(void){
 	// for(uchar i=0; i<8; i++){
		// in[i] = 20+i*3;
	// } 

	uchar c=0;
	
	initShift(5,4,3,2);
	midi_init();
	DDRB |= 1;
	sei();

	for(;;){
		
		c++;
		if(c==ROWS){ c=0; PORTB ^= 1;}
		
		in[c] = shift(_BV(c));
		
		uchar i=8;
		do{i--;
		
			bool in_bit = rbi(in[c],i);
			bool last_bit = rbi(last[c],i);
			if(in_bit != last_bit){
				uchar note = (c*8 + i);
				if(in_bit){
					if(poly>=POLY)
						continue;
					poly++;
					note_on(note ,127);
					}
				else{
					poly--;
					note_off(note);
					}
			}
		}while(i);
		
		last[c] = in[c];
	}
	return 0;
}

// ISR(TIMER1_COMPA_vect){
	// if(cnt <= steps[cstep].len)
	// PORTB ^= 1;
	// else
	// PORTB &= ~1;

// }