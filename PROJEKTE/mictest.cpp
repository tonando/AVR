#include "Macros.h"
#include "Anal.h"
// #include "math.h"


#include <util/delay.h>

int main(void){ 

   DDRB |= _BV(3);
   DDRB |= _BV(4);      
   PORTB |= _BV(3);      
   
   int a = analRead(4);      
   while(1)
    { 
	a = analRead(4);
	do{
 	_delay_ms(1); 
}while(--a);
      PORTB ^= _BV(3);
	  
	}
 }
  