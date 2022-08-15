

#include "Macros.h"
#include "stdlib.h"
#include "tiny_serial.h"
#include <stdio.h>

#define DCHA	220
#define DCHB	60
#define DTOL	10

volatile uchar pwr[2];
volatile bool last_rx;
volatile int del,treffer;
volatile int last_cnt=0;

volatile int last_on,last_off,last_period;

void print_int(uint _int){
	char* buffer= (char*) calloc(6,sizeof(char));
	
	sprintf(buffer, "%d ", _int);
	serial_print(buffer);
}	

int main(void){
	
	BUTTON(1);
	BUTTON(2);
	OUTPUT(0);
	GIMSK |= B(PCIE);
	PCMSK |= B(PCINT1);
	TCCR1 = 2;
	TCNT1 = 0;
	
	tiny_serial_init();
	_delay_ms(500);
	sei();
	serial_print("Hi!\n\r");
	
	while(1){
		/* 
		if(READ(2)){
			print_int(last_on);			
			print_int(last_off);			
			print_int(treffer);			
			
		}
		else{
		
		}
		
		serial_print("\n\r");
		 */
		if(last_on == 97 && !rbi(PORTB,0))
			ON(0);
			
		if(last_on == 32 && rbi(PORTB,0))
			OFF(0);
		
		_delay_ms(20);
			
	}

	return 0;
}

SIGNAL(PCINT0_vect){
	uchar cnt=TCNT1;
	TCCR1 = 0;
	TCNT1 = 0;
	
	if(cnt > 15 && cnt < 210){

		bool rx=READ(1);
		
		if(rx){
			last_period = last_on + last_off;
			
			last_off=cnt;
		}
		else{
			if(last_off + cnt == last_period)
				treffer++;
			else
				treffer = 0;

					
			if(treffer == 10)
				serial_print("passt \n\r");
				
			last_on=cnt;
		}
			
		last_cnt=cnt;
		last_rx=rx;
		
	}
	
	TCCR1 = 2;
}