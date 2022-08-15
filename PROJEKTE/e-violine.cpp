
// #define MCU attiny44a
#include "Macros.h"
#include "math.h"
#include "eeprom.h"

#define TONE_OUT_PIN 	PA6
#define VOL_OUT_PIN 	PA7
#define BOW_IN_PIN 		PB2
#define A_FRET 			0
#define A_STRING 		1

#define ADC_RES		256

#define G3 196.0f
#define D4 293.7f
#define A4 440.0f
#define E5 659.3f

#define G_STEP ((G3 * 3) / ADC_RES)
#define D_STEP ((D4 * 3) / ADC_RES)
#define A_STEP ((A4 * 3) / ADC_RES)
#define E_STEP ((E5 * 3) / ADC_RES)

#define MIN_DELAY 379-10
#define MAX_DELAY 2551+10

volatile uint bow;
// volatile uint micros;
uint ain,a_last;

double f_lowest=A4;		// 
double f_step=A_STEP;

uint c_delay;			// tone delay
uchar c_string;	// saite

void set_ocr(uint);
void set_tone(uint);
void set_string(uchar s);

int main(void){
	
	uint cnt=0;
	// uint test=0;

	DDRA = B(TONE_OUT_PIN) | B(VOL_OUT_PIN);		
	BUTTON(BOW_IN_PIN);	// INT0

	// vol pwm
	TCCR0A = B(COM0B1) | 3;
	TCCR0B = 1;
			
	// tone
	TCCR1A = B(COM1A0) | 4;
	TCCR1B = B(WGM12) | 1;

	// bow
	MCUCR |= 3; 
	GIMSK |= B(INT0);

	set_ocr(100);
	// set_string(0);
		
	sei();
	
	while(1){
		
		// test ++;
		// test&=4095;
		
		ain = adc_read10(A_FRET);
		
		if(ain != a_last){
			set_ocr(ain<<5);			
			a_last = ain;
		}
				
		cnt++;
		cnt&=2047;
		
		if(!cnt && bow) 
			bow--;
		
		OCR0B=bow;
	}
	return 0;
}


void set_string(uchar s){
	s&=3;
	if(c_string == s)
		return;
		
	switch(s){
		
		case 0: f_lowest = G3; f_step = G_STEP; break;
		case 1: f_lowest = D4; f_step = D_STEP; break;
		case 2: f_lowest = A4; f_step = A_STEP; break;
		case 3: f_lowest = E5; f_step = E_STEP; break;
	}

	c_string = s;	
}

void set_ocr(uint _v){
	_v = 20000 - _v;
	OCR1AH = _v >> 8;
	OCR1AL = _v & 255;
}

void set_tone(uint a){
	
	double f = 8 / fma(a,f_step,f_lowest);
	c_delay = round(f * 100000);

}

volatile uchar ovf=0;

// ISR(TIMER0_OVF_vect){
	// ovf++;
	// ovf&=255;
	
	// if(bow > ovf)
		// sbi(PORTA,VOL_OUT_PIN);
	// else
		// cbi(PORTA,VOL_OUT_PIN);
// }
ISR(INT0_vect){
	if(bow < 255)
		bow++;	
}