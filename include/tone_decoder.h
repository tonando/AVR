#ifndef TONE_DECODER_h
#define TONE_DECODER_h

#include <Macros.h>

#define TIMEOUT 	2000	// 200ms
#define TOLERANCE	6		// counts
#define MATCHES		4		// 

// 100 us
#define PRESCALER 	8 		
#define OCR_VALUE 	100		

#define INT_PIN		2		// INT0

volatile uchar tone_matches;
volatile uint dec_cnt;
volatile uint last_dec_cnt;

uint current_tone;

void tone_decoder_init(){
		cli();
		
		INPUT(INT_PIN);
		OUTPUT(1);

		GIMSK = B(INT0);
		MCUCR = 2; 

		TCCR1 = B(COM1A0) | PRESCALER;
		TIMSK |= _BV(OCIE1A);
		
		OCR1A = OCR_VALUE;
		tone_matches=0;
		dec_cnt=0;
		
		sei();
}

bool is_match(uint _d){
	if(_d > (last_dec_cnt-TOLERANCE) && _d < (last_dec_cnt+TOLERANCE))
		return true;
	
	return false;
}
void tone_found(uint _t){
	current_tone = _t;
	tone_matches = 0;
	
	GIMSK = 0;
}
uint get_tone(){
	uint result = current_tone;
	current_tone = 0;
	GIMSK = B(INT0);

	return result;
}

ISR(INT0_vect){	

	uint d = dec_cnt;
	dec_cnt = 0;
	
	if(is_match(d)){
		tone_matches++;
		if(tone_matches >= MATCHES)
			tone_found(d);
	}
	
	// else
		// tone_matches
	
	last_dec_cnt = d;
	TCNT1 = 0;
}

ISR(TIM1_COMPA_vect){
	if(dec_cnt < TIMEOUT)
		dec_cnt++;
}
#endif