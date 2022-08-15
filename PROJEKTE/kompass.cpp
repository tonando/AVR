#include <Macros.h>
#include <util/delay.h>
#include <math.h>

int main(){ 
    DDRB = 0b00000111;
	PORTB |= _BV(PB2) | _BV(PB3);

	sei();
	
   while (1){

   }
      
   return 0;
}



// ISR(TIMER0_OVF_vect){}

// ISR(ADC_vect){
	// ain = 120; //ADCH;
	// ain = ~(1 << (ADCH >> (8-RESOLUTION)));
// }