#include <util/delay.h>
#include <Macros.h>
// #include <EEPROM.h>
	
#define MOTOR_PIN 	PB1
#define BTN_START 	PB0		// hold: load last from eeprom

#define STEPS	170
#define MICROS 	6	//(1000 / STEPS)
// #define LENGTH	MICROS * STEPS
#define OFFTIME	19 //LENGTH - MICROS * STEPS
#define ONTIME	1


void set_speed(uchar);
void write(uchar);
uint pos;
// volatile uint turns = 50;

	uchar adc(){
		// cli();
		ADCSRA |= (1<<ADSC);					//Start conversion
		loop_until_bit_is_set(ADCSRA, ADSC);	//Wait for conversion complete
		// ADCSRA |= (1<<ADIF);			
		
		// uint result = MERGE(ADCH,ADCL);
		// sei();
		return (ADCH);
	} 

int main (void)	{	


	// TCCR0A = _BV(COM0B1) | (3); //fast pwm
	// TCCR0B = 6;
	
	// GIMSK |= _BV(PCIE);
	// PCMSK |= _BV(PCINT0);
	uchar cnt=0;
	DDRB = _BV(MOTOR_PIN);
	// PORTB |= _BV(BTN_START);
	ADCSRA = _BV(ADEN) | _BV(ADPS1);
	ADMUX=_BV(ADLAR) | 2;				//Conversion on channel 0, AVCC reference,  10 bit mode
	pos=0;
	bool dir = false;
	// sei();
		
	while( 1 ) {
		ON(MOTOR_PIN);
		_delay_ms(ONTIME);
		
		uint d = pos * MICROS;
		
		delay_us(d);
		OFF(MOTOR_PIN);
		
		d = (STEPS-pos)*MICROS;
		delay_us(d);
		
		_delay_ms(OFFTIME);
		
		cnt++;
		cnt&=63;
		if(!cnt){
			if(dir) pos++;
			else pos--;
			pos &= 127;
			if(!pos) dir = !dir;
			
			// pos=adc();
			// pos=constrain(pos,0,STEPS);
		}
	}
	return 0;
}
// ISR(PCINT0_vect){
// ISR(PCINT0_vect){
	// if(!READ(0)){

	// }
// }

