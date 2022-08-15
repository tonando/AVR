
#include "Macros.h"

#define START 		 4
#define END 		 4
#define	STEP 		2
#define TOTAL 		START + END + (1023*STEP)

#define SERVO_PIN	PB1

volatile uint cnt=0;
uint val=0;
bool pressed;

uint read_adc(void){
	
	// set up ADC
    ADMUX = 2 | _BV(ADLAR);
	ADCSRA = _BV(ADEN) | _BV(ADEN);//
// Start the conversion
	// sbi(ADCSRA,ADSC);
    // Wait for it to finish
    while (rbi(ADCSRA,ADSC));
	// uint result = ADCH;
	return ADCH;
    // return (result);
}

int main(void)
{

	DDRB = _BV(SERVO_PIN);
	ON(0);

	TCCR0B = 2;
	TIMSK0 |= _BV(TOIE0);
	
	// OFF(SERVO_PIN);
	
	// _delay_ms(10);	
	sei();

	while(1)
	{
		// pressed = READ(0);
		// if(pressed){
			val = read_adc();
			val *= STEP;
		// }
		_delay_ms(10);
	}

	return 0;
}

ISR(TIMER0_OVF_vect){
	cnt++;
	if(cnt >= TOTAL)
		cnt=0;
	// cnt &= 255;
	
		// OFF(SERVO_PIN);
	// if(pressed){
		
		// if(cnt < START)
			// OFF(SERVO_PIN);
		// else{
		// if(cnt < START)
			// OFF(SERVO_PIN);
			
			if(val > cnt)
				ON(SERVO_PIN);
			else
				OFF(SERVO_PIN);
	// }
		// }
}