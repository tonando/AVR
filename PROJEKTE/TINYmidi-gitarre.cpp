
#include "Macros.h"
#include "stdlib.h"
#include "tiny_serial.h"
#include <stdio.h>

#define E_STR	40
#define A_STR	45
#define D_STR	50
#define G_STR	55
#define B_STR	59
#define e_STR	64

#define NUM_FRETS	24
#define NUM_STRINGS 6
#define SUSTAIN		100

#define ADIV	(255 / NUM_FRETS)

uchar strings[NUM_STRINGS];
volatile uint timeouts[NUM_STRINGS];
 uchar volume[NUM_STRINGS];
volatile uchar picked,last_picked;	//bitmask

void string_off(uchar s);

int main(void){
	
	
	BUTTON(0);
	INPUT(3);
	
	// GIMSK |= B(PCIE);
	// PCMSK |= B(PCINT1);
	tiny_serial_init();
	
	TCCR1 = _BV(CTC1) | 9;
	TIMSK |= _BV(OCIE1A);
	OCR1A = 156;
	// TCNT1 = 0;
	
	uchar cnt=1;
	
	_delay_ms(50);
	
	sei();
	
	while(1){
		if(!READ(0) || !cnt){
			while(!READ(0));
			
			if(rbi(picked,0)){
				string_off(0);
			}
			uint note = adc_read(3);
			note /= ADIV;
			
			strings[0] = (uchar)note  + E_STR;
			timeouts[0]=SUSTAIN;
			sbi(picked,0);			

			char o[4];
			o[0] = (char)(0x90);
			o[1] = (char)(strings[0]);
			o[2] = (char)(123);
			// o[3] = '\n';
			
			serial_print(o);
			
				
		}
		else{
			_delay_ms(10);
			
		}
			cnt++;
	}

	return 0;
}

void string_off(uchar s){
			cbi(picked,s);
			timeouts[0]=0;
			
			char o[4];
			o[0] = (char)(0x80);
			o[1] = (char)(strings[s]);
			o[2] = (char)(0);
			// o[3] = '\n';
			
			serial_print(o);
			// serial_write(0x80);
			// serial_write(strings[s]);
			// serial_write(s);
			
			//  _delay_ms(5);
}

SIGNAL(TIMER1_COMPA_vect){

	if(timeouts[0] && rbi(picked,0)){
		timeouts[0]--;
		
		if(!timeouts[0]){
			string_off(0);
		
		}
	}
}