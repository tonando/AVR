
#include <Macros.h>
#include "tiny_midi.h"


int main (void)
{ 
	OUTPUT(2);
	OUTPUT(1);
	INPUT(0);
	ON(0);
	
	tiny_serial_init();	// 9600
	
	while(1){
		if(besetzt)
			ON(2);
		else
			OFF(2);
			
		if(!READ(0)){
			ON(1);
			note_on(77,99);
			_delay_ms(100);
			note_off(77);
			_delay_ms(100);
			OFF(1);
		}
		
	}
	
	return 0;
}
