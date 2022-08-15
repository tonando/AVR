
#include "Macros.h"
#include "stdlib.h"
// #include "Anal.h"

// #define BAUD 31250
// #include "midi.h"
// #include "eeprom.h"
#include "util/delay.h"

int main(void){
	uint cnt=0;
	DDRB = 0xff;

	TCCR0B = 0x01;
	TIMSK = _BV(TOIE0); // enable CTC interrupt
	
	// midi_init();
	sei();
	while(1){
	cnt++;
	cnt&=31;

	// if(midi_in[1])
		// OCR1A = midi_in[1] * 200 + 2000;
	// else
		// OCR1A = 29000;

	}

	return 0;
}

ISR(TIM0_OVF_vect){

		PORTB ^= 1;

}