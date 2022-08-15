#include <Macros.h>
#include <util/delay.h>
#include <math.h>

 // #define PWM_DIV 4
// #include <shiftOut.h> 
// #include <Anal.h>

// #define REGS 2
// #define RESOLUTION 16 //(8 * REGS)
#define LEDS 16
#include "IMAGES.h"

#define start_adc    sbi(ADCSRA,ADSC);
#define BLINK		for(uchar i=0; i<3; i++){ PORTB ^= 2; _delay_ms(100); }
#define MOSI 	1
#define LATCH 	0
#define CLOCK 	2
#define BUTTON 	3

//const unsigned char SINTABLE[32] = {8, 9, 11, 12, 13, 14, 15, 15, 16, 15, 15, 14, 13, 12, 11, 9, 7, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6};

//volatile uchar ain=4,cnt=0;
//uint output;
uchar mode;
//bool last_bt, fill;

void shift(uchar);
void setupAdc();


void setupAdc(){
  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX = 2;    // set 2 analog input pin
  ADMUX |= _BV(REFS0);  // set reference voltage
  ADMUX |= _BV(ADLAR);  // left align ADC value to 8 bits from ADCH register

  ADCSRA |= _BV(ADPS2) | _BV(ADPS0);    // 32 prescaler for 38.5 KHz
  // ADCSRA |= _BV(ADPS2);                     // 16 prescaler for 76.9 KHz
  //ADCSRA |= _BV(ADPS1) | _BV(ADPS0);    // 8 prescaler for 153.8 KHz

  ADCSRA |= _BV(ADATE); // enable auto trigger
  ADCSRA |= _BV(ADIE);  // enable interrupts when measurement complete
  ADCSRA |= _BV(ADEN);  // enable ADC
  sei();
  start_adc;
}

int main(){ 
   
   DDRB |= 0x0f;
   //PORTB |= 0xf0;
   
   
   TCCR0B = 2;
   TIMSK = _BV(TOIE0);
    sei();
   //setupAdc();
   uchar cnt=0;
   while (1){
		cnt++; 
		//cnt &= 15;
		if(cnt == 50)
			PORTB ^= _BV(2);
		if((cnt & 63) == 0)
			PORTB ^= _BV(1);
		}
      _delay_ms(200);
   return 0;
}



ISR(TIM0_OVF_vect){
	///if(speed )
	PORTB ^= _BV(2);
		}

ISR(ADC_vect){
	uint tmpa = ADC;	
   start_adc;
} 