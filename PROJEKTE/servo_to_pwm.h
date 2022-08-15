
#include <Macros.h>
#include "tiny_serial.h"

long val,old_val;

int main (void)
{ 

	tiny_serial_init(80);
	serial_print("OK\n\r");
	
	uchar cnt=0;
	
	MCUCR |= (1 << ISC01 );
	GIMSK |= (1 << INT0);	
	
	sei();
	
	while(1){
			cnt++;
			
			if(val != old_val){
				serial_print_int(val);
				serial_print("\r\n");
				old_val=val;
			}
	}
	
	return 0;
}

ISR(INT0_vect){
	_delay_ms(1);
	
	for(val=0; READ(2); val++);
}