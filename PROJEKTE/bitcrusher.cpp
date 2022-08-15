/**
 * Copyright (c) 2019, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * ATtiny13/052
 * Example of 8bit mono class D amplifier.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
// #include <util/delay.h>

#define	AUDIO_IN	PB3
#define	AUDIO_OUT	PB0
#define	BTN			PB2

volatile unsigned char crush = 0;

int
main(void)
{

	/* setup */
	DDRB = _BV(AUDIO_OUT); // set AUDIO_OUT pin as output
	// PORTB |= _BV(BTN);
	TCCR0A |= _BV(WGM01)|_BV(WGM00); // set timer mode to FAST PWM
	TCCR0A |= _BV(COM0A1); // connect PWM pin to channel A
	TCCR0B |= _BV(CS00); // set prescaler to 1
	ADMUX |= _BV(MUX0)|_BV(MUX1); // select ADC3 (PB3)
	ADMUX |= _BV(REFS0); // set internal 1.1V reference voltage
	ADMUX |= _BV(ADLAR); // left adjust of ADC result
	ADCSRA |= _BV(ADPS1)|_BV(ADPS0); // set ADC division factor to 8
	ADCSRA |= _BV(ADEN)|_BV(ADIE); // enable ADC and interrupt
	ADCSRA |= _BV(ADATE); // set ADC auto-trigger
	ADCSRA |= _BV(ADSC); // start conversion
	
	// GIMSK |= _BV(INT0);
	// MCUCR |= 2;
	// PCMSK |= _BV(BTN);
	
	sei(); // enable global interrupts

	/* loop */
	while (1);
	
	return 0;
}

/* 		
ISR(INT0_vect){
	// if(!bit_is_set(PINB,BTN)){
		OCR0A=0;
		cli();
		crush++;
		if(crush > 3) crush=0;
		for(char i=0; i<100; i++){
			if(i&1)
				OCR0A = 40;
			else
				OCR0A = 0;
		}
		// _delay_ms(100);
		while(!bit_is_set(PINB,BTN));
		sei();
	// }
}
		 */

ISR(ADC_vect)
{
	// if(crush)
		// OCR0A = (ADCH>>crush)<<crush; // the magic
	// else
		OCR0A = ADCH; // the magic
}