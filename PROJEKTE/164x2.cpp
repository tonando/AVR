// 9.6 MHz, built in resonator
// #define F_CPU 9600000
//#define LED PB0
 
 
#include <avr/io.h>
#include <util/delay.h>
#include <SINTABLE.h>
#include <Anal.h>
 /* 
void adc_setup (void)
{
    // Set the ADC input to PB2/ADC1
    ADMUX |= 2;
    // ADMUX |= (1 << ADLAR);
 
    // Set the prescaler to clock/128 & enable ADC
    // At 9.6 MHz this is 75 kHz.
    // See ATtiny13 datasheet, Table 14.4.
    ADCSRA |= (1 << ADEN);
}
 
unsigned char adc_read()
{
    // Start the conversion
	    // ADMUX |= _BV(ch);
    ADCSRA |= (1 << ADSC);
 
    // Wait for it to finish
    while (ADCSRA & (1 << ADSC));
 
    return ADCH;
}
  */
void pwm_setup (void)
{
    // Set Timer 0 prescaler to clock/8.
    // At 9.6 MHz this is 1.2 MHz.
    // See ATtiny13 datasheet, Table 11.9.
    TCCR0B |= (4);
 
    // Set to 'Fast PWM' mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
 
    // Clear OC0B output on compare match, upwards counting.
    TCCR0A |= (1 << COM0B1);
}
 
void pwm_write (unsigned char val)
{
    OCR0B = val;

}
 
int main (void)
{
    // LED is an output.
    DDRB = 3;  
	PORTB |= PB2;

    pwm_setup();
	unsigned char a,b;
	unsigned char i=1;
    while (1) {
		
			
		i++;
		i &= SINSIZE-1;
		

        // Get the ADC value
		
		// if(i){
			a = analRead(3)>>4;
		// }
		// else{
			// poti = adc_read(1) & 255;

        // Now write it to the PWM counter
		
		//b = (i);
		b = SINTABLE[i];
/* 		
		if(a > 200 && b < 100)
			b = 0;
		
		
		if(b < 40 && a < 20)
			b <<= 1; */

        pwm_write(b);
		PORTB ^= 1;
		
		if(PINB & _BV(PB4) == 0){
			for(int k=0; k<4; k++){
			PORTB ^= 1;	
			_delay_ms(k==3?200:50);			
			PORTB ^= 1;
			_delay_ms(k==3?200:50);
			}
			continue;
		}
		for(int j=0;j<a; j++)
			_delay_ms(10);

    }
}