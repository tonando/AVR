// HEADER

#define F_CPU 16000000UL
#define BAUD  14400
#include "Macros.h"
#include "Anal.h"
#include "shiftinout.h"
// #include "serial.h"
#include "midi.h"
#include "util/delay.h"

#define NSTEPS 8

#define TONE_PIN PB0

#define MIDI_START 33
#define MIDI_A4 69

const uint freqvals[12] = {
36363,34334,32388,30592,28880,27257,25723,24279,22922,21621,20408,19253};


typedef struct{
	bool on;
	uchar note;
	uchar len;
	uchar vel;
}step;

step steps[NSTEPS];

char last_note;
uchar cstep,cnt;
uchar in;

uchar tempo=200;
// bool recording;

void setFreq(uint f){
	f /= 2;
	uint ttt = (F_CPU / f);
	
	if (TCNT1 > ttt)
		TCNT1 = 0;
	OCR1A = ttt;
}

void bd(){
	OCR1A= (1 + cnt) * ( 10 + (cstep & 3) * 10);
	OCR1A <<= 1;
}

void playNote(step* s){
	uint f = freqvals[s->note%12];
	f /= 1 + (s->note / 12);
	OCR1A = f;
	// TCCR1B = _BV(WGM12) | _BV(CS10);
s->on = 1;
	note_on(s->note,s->vel);
}

int main(void){
 	for(uchar i=0; i<NSTEPS; i++){
		steps[i].on = true;
		// steps[i].note = 20+(i*2)+(i&1);
		steps[i].note = 69+i+(i&1);
		// if(i%7 == 0)
			// steps[i].note = 10;
		steps[i].len = 127 >> (i&3);
		steps[i].vel = 100;
	} 
	
	// TCCR1A = _BV(WGM11); // enable CTC
	TCCR1B = _BV(WGM12) | 0x02;
	TIMSK1 = _BV(OCIE1A); // enable CTC interrupt
	
	analInit(0);
	// DDRC = 0;
	initShift(5,4,3,2);
	
	// uart_init();
	midi_init();

	DDRB |= 1;//TONE_PIN;
	DDRD &= 0x0f;
	PORTD |= 0xf0;
	
	sei();
	playNote(&steps[1]);
	// setFreq();	

	while(1){

	
		// input
		bool recording = !rbi(PIND,7);
			// TCCR1B = _BV(WGM12);
		if(!rbi(PIND,6)){
			bd();
			_delay_ms(10);
			continue;
		}
		// }		
		// else
			// tempo=analRead(1) >> 2;
		

		step* s = &steps[cstep];
		
		cnt++;
		cnt &= 127;
		
		
		if(cnt == s->len){
			s->on = false;
			note_off(s->note);
		}
		
		if(!cnt ){
		
			cstep++;
			if(cstep == NSTEPS)
				cstep = 0;
				
			if(recording){
				uchar ain = analRead(0) >> 5;
				ain += 20;
				s->note = min(60,ain);	
			}

			
			
			playNote(s);
			
			// last_note = s->note;
		}
		in = shift(~(_BV(cstep) | in));
		
		for(uchar t = 0; t < tempo; t++)
			_delay_us(32);
		
	}
	return 0;
}

ISR(TIMER1_COMPA_vect){
	if(steps[cstep].on)
		PORTB ^= 1; //TONE_PIN;	
	else
		PORTB &= ~1;

}