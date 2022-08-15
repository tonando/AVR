
#include <Macros.h>
// #include "tiny_serial.h"

#define LOCK 	PB4
#define BUZZ 	PB2
#define LEDR 	PB1
#define LEDG 	PB0

uchar CODE[3] = {8,1,4};
uchar cnum;

uint timeout;

uchar ain,lain;

int main (void)
{ 

	OUTPUT(LOCK);
	OUTPUT(BUZZ);
	OUTPUT(LEDR);
	OUTPUT(LEDG);
	
	uchar cnt=0;
	
	_delay_ms(100);
	
	sei();
	
	while(1){
			
			ain = adc_read(3);


			
			cnt++;
			lain=ain;
	}
	
	return 0;
}
