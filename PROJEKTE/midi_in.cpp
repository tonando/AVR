
#include "Macros.h"
#include "stdlib.h"
#include "Anal.h"

#define BAUD 31250
#include "midi.h"
// #include "eeprom.h"
#include "util/delay.h"

int main(void){
	uint cnt=0;
	DDRB = 0xff;

	TCCR1B = _BV(WGM12) | 0x02;
	TIMSK1 = _BV(OCIE1A); // enable CTC interrupt
	
	midi_init();
	sei();
	while(1){
	cnt++;
	cnt&=31;

	// if(midi_in[1])
		// OCR1A = midi_in[1] * 200 + 2000;
	// else
		// OCR1A = 29000;

		if(msg[0]){
		sbi(PORTB,5);
			for(uchar i=0; i<3; i++){
				
				uart_put(msg[i]);
			}
			uart_put('\n');
			uart_put('\r');
			
			midi_clear_buffer();
			_delay_ms(100);
			cbi(PORTB,5);
		}
	}

	return 0;
}

ISR(TIMER1_COMPA_vect){

		PORTB ^= 1; //TONE_PIN;	

}