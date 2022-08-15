// #define MCU attiny45

#include "Macros.h"
	// #define BAUD 9600
// #include "beep.h"
#include "eeprom.h"
// #include "util/delay.h"

#define TONE_PIN 	PB1
#define VOL_PIN 	PB0
#define BOW_PIN 	PB2
#define A_FRET 		3
#define A_STRING 	2

#define ADC_RES		1024

#define G3 196.0f
#define D4 293.7f
#define A4 440.0f
#define E5 659.3f

#define G_STEP ((G3 * 2) / ADC_RES)
#define D_STEP ((D4 * 2) / ADC_RES)
#define A_STEP ((A4 * 2) / ADC_RES)
#define E_STEP ((E5 * 2) / ADC_RES)

// #define MIN_DELAY (50000UL / ((E5) * 4))
#define MIN_DELAY 379-10
#define MAX_DELAY 2551+10
// #define MAX_DELAY (50000UL / (G3))

volatile uchar bow;
uint ain,a_last;


// uint a_min=0,
	// a_max=1023,
	// a_mid=511,
	// a_last=0;
float f_lowest=A4;	// 
float f_step=A_STEP;
	
uchar c_string;	// saite
ulong c_delay;			// tone delay

void set_tone(uint);
void set_string(uchar s);

int main(void){
	
	
	OUTPUT(TONE_PIN);
	OUTPUT(VOL_PIN);
	
	BUTTON(BOW_PIN);	// INT0
	// INPUT(BOW_PIN);	// INT0
	INPUT(ATOP(A_STRING));
	INPUT(ATOP(A_FRET));
	// OUTPUT(ATOP(A_STRING));
	// volume pwm
	TCCR0A =  B(COM0A1) | 3;
	TCCR0B = 3;
	OCR0A = 0;
	
	// slow
	TCCR1 = 9;
	TIMSK |= B(TOIE1);
	
	MCUCR |= 3; 
	GIMSK |= B(INT0);
	

	ADMUX = A_FRET;	
	ADCSRA = /* B(ADLAR) |  */B(ADSC) | B(ADEN) | B(ADPS1) | B(ADPS0) ;	//set diviser to 8
	// ADCSRA |= B(ADEN);	// | (1<<ADATE);              // free running mode
    
	// for(uint i=0; i<1024; i++){
		// set_tone(i);
		// fbi(PORTB,TONE_PIN);
		// delay_us(c_delay);
	// }
	TCNT0 = 0;
	TCNT1 = 0;
	
	
	set_string(c_string);
	set_tone(0);
	uchar cnt=0;
	
	sei();
	
	// _delay_ms(10);
	// ADCSRA |= B(ADSC);
	
	while(1){
		

	
	if( !rbi(ADCSRA,ADSC)){
	// ain = ((ADCH<<8)&0xff) | (ADCL&0xff);
		ain = ADCL;
		ain |= ADCH<<8;
		// noise
		// ain &= ~1;
		if(ain != a_last){
			// ON(ATOP(A_STRING));
			if(ADMUX == A_STRING)
				set_string(ain>>8);
			else
				set_tone(ain);
			// OFF(ATOP(A_STRING));
			a_last = ain;
		}
			
		cnt++;
		cnt&=7;
		
		if(!cnt) ADMUX = A_STRING;
		else ADMUX = A_FRET;
		
		ADCSRA |= B(ADSC);
	}

	

		// if(!READ(BOW_PIN)) bow=255;
		// else OCR0A = bow;
		// OCR0A = 255-bow;
		
		if(c_delay){
				
			for(uchar i=8; i; i--){
				ON(TONE_PIN);
				delay_us(c_delay);
				OFF(TONE_PIN);
				delay_us(c_delay);
			}
			
		
		}
		
		
	}
	return 0;
}


void set_string(uchar s){
	switch(s){
		case 0: f_lowest = G3; f_step = G_STEP; break;
		case 1: f_lowest = D4; f_step = D_STEP; break;
		case 2: f_lowest = A4; f_step = A_STEP; break;
		case 3: f_lowest = E5; f_step = E_STEP; break;
	}
}
 void error(){
	cli();
	// for(uchar i=10; i; i--){
	while(1){
		ON(TONE_PIN);
		OCR0A=255;
		delay_ms(100);
		OCR0A=0;
		OFF(TONE_PIN);
		delay_ms(100);
	}
	// sei();
}

void set_tone(uint a){
	
	// ulong c_freq = f_lowest + (a * f_step);
	// float f = c_freq / 10000;
	float f = (a * f_step) + f_lowest;
	// f += f_lowest;
	// f/=1000.0f;
	c_delay = 500000UL;
	c_delay /= f;
	// c_delay /= 2;
/* 	
if(c_delay < MIN_DELAY || c_delay > MAX_DELAY){
		uchar e = c_delay & 0xff;
		EEPROM_write(3,e);

		c_delay >>= 8;
		e = c_delay & 0xff;
		EEPROM_write(2,e);
		
		c_delay >>= 8;
		e = c_delay & 0xff;
		EEPROM_write(1,e);
		
		c_delay >>= 8;
		e = c_delay & 0xff;
		EEPROM_write(0,e);

		c_delay=0;
		error();
} 
 */	
}
/* 
ISR(ADC_vect){
	ain = ((ADCH<<8)&0xff) | (ADCL&0xff);
	// noise
	// ain &= ~1;

	if(ain != a_last){
		// ON(ATOP(A_STRING));
		if(ADMUX == A_STRING)
			set_string(ain>>7);
		else
			set_tone(ain);
		// OFF(ATOP(A_STRING));
		a_last = ain;
	}
	
	cnt++;
	cnt&=3;
	
	if(!cnt) ADMUX = A_STRING;
	else ADMUX = A_FRET;

	ADCSRA |= B(ADSC);
}
 */
ISR(INT0_vect){
	if(bow < 255)
		bow++;
	// else 
		// bow=255;
	
	OCR0A = bow;		
}	

ISR(TIMER1_OVF_vect){
	if(bow)
		bow--;
		
	OCR0A = bow;				
	// else
		// OFF(VOL_PIN);
		// TCNT0 = 0;
	// }
	
}
	
	
	
	
// ISR(TIMER0_COMPA_vect){
	// PORTB ^= _BV(VOL_PIN);

// }
// ISR(TIM0_COMPA_vect){
	// ovf++;
	// if(ovf<=bow)
		// PORTB ^= _BV(TONE_PIN);
		// PORTB |= 2;
	// else
		// PORTB &= ~2;
		// }
// ISR(TIMER1_COMPA_vect){
	// if(cnt <= steps[cstep].len)
	// if(bow)
	// PORTB ^= 1;
	// }
	
	/* 
// uint map(uint x, uint in_min, uint in_max, uint out_min, uint out_max)
uint map(uint x)
{
	uint out_min=strings[c_str].high;
	uint out_max=strings[c_str].low;
  if ((a_max - a_min) > (out_max - out_min)) {
    return (x - a_min) * (out_max - out_min+1) / (a_max - a_min+1) + out_min;
  }
  else
  {
    return (x - a_min) * (out_max - out_min) / (a_max - a_min) + out_min;
  }
}
 */
