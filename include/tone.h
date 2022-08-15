#ifndef TONE_H
#define TONE_H

#define CPU 8000000UL

#if CPU == 8000000UL	
	#define T_OCT 11
#else
	#define T_OCT 8
#endif

#define DIVIDER (CPU / 16)

#define TONE_TIMER	0
#if TONE_TIMER == 1
#define T_TCCR 		TCCR1
#define T_OCR 		OCR1C
#define T_SETTING	0x90
#define	T_TIMSK		TIMSK1 
#define	T_OCIE		OCIE1
#define T_INT		TIMER1_OCR1_vect
#else
#define T_TCCR 		TCCR0
#define T_OCR 		OCR0A
#define T_SETTING	0x90
#define	T_TIMSK		TIMSK 
#define	T_OCIE		OCIE0
#define T_INT		TIMER0_OCR0_vect
#endif

char tone_out_pin=-1;
bool tone_on=false;
uchar tone_octave=0;
uchar divisor=0;

void tone_init(uchar _p){
	tone_out_pin = _p;
	DDRB |= _BV(tone_out_pin);
	T_TIMSK = _BV(T_OCIE);
}
void tone_play(uint _f){
	if(tone_out_pin == -1)
		return;
	
	f>>=1;
	ulong div = DIVIDER / f;
	divisor = (uchar)div;

	T_TCCR = T_SETTING | (T_OCT - octave);
	T_OCR = divisor-1;         // set the OCR

	#else
	
}
void tone_stop(uint _f){
	tone_on = false;
	OFF(tone_out_pin);
	T_OCR = 0;
}
ISR(T_INT){
	if(T_OCR){
		PORTB ^= _BV(tone_out_pin);
	}
	else{

	}
	
}
// void tone_setup(uchar _p){}


#endif