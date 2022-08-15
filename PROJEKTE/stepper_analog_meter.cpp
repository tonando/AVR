#include <util/delay.h>
#include <Macros.h>
#include <StepperMotor.h>


#define IN 4 // a 3

void calibrate(void);
// void left(void);
// void right(void);
uchar read_adc(void);
uchar ain,last_ain;

// const uchar STEPS[8] = {0b10101010,0b00100010,0b01100110,0b01000100,0b01010101,0b00010001,0b10011001,0b10001000};
StepperMotor s;
// uint total_steps = 0;
// int pos = 0;
// bool calibrated = false;
// uchar c_step = 0;

int main (void)	{	
	
	DDRB = 0x0f;
	uchar cnt=0;
	// PORTB = (0x0f & STEPS[0]) | _BV(IN);
	// s = new StepperMotor();
	// calibrate();
	
	// set up ADC
    ADMUX |= (1 << ADLAR);
    ADMUX |= 2;
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);

	sei();
	// uchar cnt=0;
	
	while( 1 ) {
		cnt++; cnt&=255;
		if(!cnt){
			ain = read_adc();
			int diff = last_ain - ain;
			
			if(diff){
				s.steps(diff);
			}
		}
		
		
	}
	return 0;
}
uchar read_adc(void){
	// Start the conversion
    ADCSRA |= (1 << ADSC);
 
    // Wait for it to finish
    while (ADCSRA & (1 << ADSC));
 
    return ADCH;
}
 /* 
 void calibrate(void){
	 while(READ(IN))
		left();
	
	_delay_ms(100);
	
	total_steps = 0;
	uchar soft = 20;
	
	 while(READ(IN) || soft){
		soft--;
		total_steps++;
	 
	 }
	
	_delay_ms(100);
	calibrated = true;
 }
 
void left(void){
	if(pos <= 0 && !calibrated) return;
	pos--;

	c_step--;
	c_step&=7;
	
	PORTB &= 0xf0;
	PORTB |= STEPS[c_step];
	
	_delay_ms(1);
	PORTB &= 0xf0;
	_delay_ms(1);
	
}
void right(void){
	if((uint)pos >= total_steps && !calibrated) return;
	pos++;

	
	c_step--;
	c_step&=7;
	
	PORTB &= 0xf0;
	PORTB |= STEPS[c_step];
	
	_delay_ms(1);
	PORTB &= 0xf0;
	_delay_ms(1);
}
 */