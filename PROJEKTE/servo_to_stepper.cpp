
#include <Macros.h>
// #include "tiny13_serial.h"
#include "stepper_motor.h"

#define DEADZONE	8
#define MIN_DEL		4200
#define MAX_DEL		240000
#define INC_DEL		120

volatile ulong val,old_val;
volatile uchar ovf;
volatile uint spd;


int main (void)
{ 
	stepper_init(3,4,0,2);
	// stepper_init(0,2,3,4);
	INPUT(1);
	
	MCUCR |= B(ISC00);
	GIMSK |= (1 << INT0);	
	
	TCCR0B=0;
	TCNT0=0;
	TIMSK=B(TOIE0);
	
	sei();
	
	while(1){
		
		signed int s = (spd - 250);
		uint sa = ABS(s);
		
		if(sa > DEADZONE){
			ulong  del = (260-sa);
			
			if(s < 0)
				step(1);
			else
				step(0);
			
			_delay_us(MIN_DEL);
			// delay_us(del);
			do{
				_delay_us(INC_DEL);
			}while(--del);
		}
		
		PORTB &= B(1);
			
	}
	
	return 0;
}

ISR(TIMER0_OVF_vect){ ovf++; }
ISR(INT0_vect){
	cli();
	if(READ(1)){
		
		// _delay_us(1200);
		_delay_ms(1);
		TCCR0B=1;
	}
	else{
		TCCR0B=0;
		val = (ovf<<8)+TCNT0;
		int v=(val-760);
		spd=constrain(v>>4,0,500);
		// spd=v>>2;
	}
	
	TCNT0=ovf=0;
	sei();
}