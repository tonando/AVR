
#include "Macros.h"
#include "stdlib.h"
#include "tiny_serial.h"
#include <stdio.h>

volatile uchar cnt,last,errors;
bool is_valid=false;
volatile uchar vcnt;


void print_int(uint _int){
	char* buffer= (char*) calloc(6,sizeof(char));
	
	sprintf(buffer, "%d ", _int);
	serial_print(buffer);
}	

int main(void){
	INPUT(PB2);
	MCUCR = 1; // any edge
	TCCR1 = 0;
	TIMSK |= _BV(TOIE1);
	// OCR1A = 50;
	tiny_serial_init();
	serial_print("hi!\n\r");
	sei();
	GIMSK = B(INT0);
	while(1){

		if(is_valid){
			// GIMSK = 0;
			cli();
			print_int(last);
			last=0;
			is_valid=false;
			serial_print("\n\r");
			// delay_ms(20);
			// GIMSK = B(INT0);
			sei();
		}
	}

	return 0;
}
/*
ISR(TIM1_COMPA_vect) { 
	cnt++;
	if(cnt>30000){
		cnt=0;
	}	
}
*/
ISR(TIM1_OVF_vect) { 
	is_valid=false;
	TCCR1 = 0;
	// errors++;
}
SIGNAL(INT0_vect) { 
	// cli();
	GIMSK = TCCR1 = 0;
	// cnt=TCNT1;	
	if(TCNT1){
		if(TCNT1 == last){
			vcnt++;
			if(vcnt >= 3){
				vcnt=0;
				is_valid=true;
			}
		}
		else{
			last = TCNT1;
		}
	}
	// cnt=0;
	TCNT1=0;
	TCCR1 = 2;
	GIMSK = B(INT0);
	// sei();
}