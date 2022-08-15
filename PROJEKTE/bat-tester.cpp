
#include <Macros.h>

#include <test.h>
// #include "tiny_serial.h"


uint BEST = 1480/20;
uint GOOD = 1390/20;
uint  BAD  = 1320/20;
uint DEAD = 1200/20;




int main (void)
{ 
sss(4);
	//tiny_serial_init(80);
	uchar cnt=0;
	DDRB |= 0b00000111;
	
	uint ain = 0;
	uint lain = 0;
	
	ain = lain = adc_read(3);
	
	if(ain > 10){
		BEST = ain;
		GOOD = BEST * 0.9;
		BAD = BEST * 0.8;
		DEAD = BEST * 0.6;
	}
//	serial_print("OK\n\r");
	
	while(1){
			cnt++;
			
			ain = adc_read(3);
			PORTB=0;
	
			if(ain){
				
				if(ain >= GOOD)
					ON(2);
				if(ain <= BAD)
					ON(0);
				if(ain > DEAD && ain < BEST)
					ON(1);	
					
		//		_delay_ms(10);
					//			serial_print_int(ain);
		//		serial_print("\n\r");
			}
			//else
			//	_delay_ms(10);
			
			lain=ain;
	}
	
	return 0;
}
