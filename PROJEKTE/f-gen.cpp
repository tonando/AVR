
#include <Macros.h>
#include <button.h>

#define OUT_PIN		PB1
#define A_FREQ		2
#define A_DUTY		3

#define DIV_MAX		3

uchar div = 2;
uchar freq = 200;

int main (void)
{
	
	init_button(0);
	sbi(DDRB,OUT_PIN);
	
	// uchar ain=freq;
	// OCR0B = freq;
	// OCR0A = freq;
	TCCR0A |=_BV(WGM01) |_BV(COM0B0) |_BV(COM0A0) ;	// hardware ctc toggle pins on match
	TCCR0B = div;
	// TCCR0B |= _BV(CS02) | _BV(CS00);					// start timer with 1024 prescaler   
	// TIMSK0 |= _BV(OCIE0);
	// OCR0A = 200;
	// uchar tcnt=0;
	// uchar dcnt=0;
	sei();
	
   while( 1 ) {
		if(button_pressed(0)){
			freq = adc_read(A_FREQ) >> 1;
			freq++;
			OCR0B = freq;
		// _delay_ms(20);
			// if(div==DIV_MAX)div=0;
			// div++;
			
			// TCCR0B = div;
		}
		
		// _delay_us(100);
		// ain=constrain(ain,1,200);
		
		// if(ain != freq){
			// freq=ain;
			// if(freq>1 && freq < 100)
			// OCR0A = freq;
		// }
/* 	   
 */		
	}	   
  

  return 0;
}
// ISR(TIMER0_COMPA_vect){
	// PORTB^=B(OUT_PIN);
// }