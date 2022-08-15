#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

void setupAdc(){
  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX |= 2;    // set 2 analog input pin
  // ADMUX |= (1 << REFS0);  // set reference voltage
  ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

//  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
   ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
  //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);  // enable ADC
  // ADCSRA |= (1 << ADSC);  // start ADC measurements
}
int main()
 { 
 	DDRB = 1; // Output on PB0/OC0A
	// Timer0, Set OC0A on comp. match (inv). Mode 3: Fast PWM
	TCCR0A = (1<<COM0A1)|(1<<COM0A0)|(1<<WGM01)|(1<<WGM00);
	// 1:8 presc.
	TCCR0B = 1;
    setupAdc();
   sei();
   ADCSRA|=_BV(ADSC);
	while (1) {
//		while(++OCR0A)
//			_delay_us(100);
	//	while(--OCR0A)
	//		_delay_us(100);
	}
   return 0;
 }
 
 
ISR(ADC_vect){
	int tmpa = ADCH;
   OCR0A=tmpa;
ADCSRA|=_BV(ADSC);
}