#ifndef BEEP_h
#define BEEP_h
// #define USE_TIMER0
#include <Macros.h>

bool beep_inited;
bool beeping;
volatile uint beep_length;
uchar beep_pin;

void beep_del(uchar _f,uchar _l);

void init_beep(uchar _p){
	DDRB |= _BV(_p);
	
	TCCR1 = _BV(CTC1) | 10;
	TIMSK |= _BV(OCIE1A);
	OCR1A = 10;
	
	beep_inited=true;
	beep_del(200,100);
}


void beep_del(uchar _f,uchar _l){
	cli();
	beeping = false;
	uint i = (255-_f) * _l;
	uint d = MAX(1,255-_f);
	d = (d<<1)+512;
	do{i--;
		ON(beep_pin);
		delay_us(d);
		OFF(beep_pin);
		delay_us(d);
	}while(i);
	sei();
	}
void beep(uchar _f,uchar _l){
	// if(beep_inited){
		beeping = false;
		beep_length = (255-_f) * _l;
		OCR1A=MAX(1,255-_f);
		beeping=true;
	// }
}
void beep(uchar _f){beep(_f,100);}

ISR(TIMER1_COMPA_vect){
	if(beeping){
		fbi(PORTB,beep_pin);
		beep_length--;
		
		if(!beep_length)
			beeping=false;
	}
}


#endif