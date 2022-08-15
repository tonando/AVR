#ifndef SIMPLE_RF433_H
#define SIMPLE_RF433_H
#include <Macros.h>

#define SEND_PIN 0
#define IN_PIN 2

#define PRESCALER 8 	// 1ms = 3

#define TIMEOUT 200
#define LED1 3


// timer
volatile uchar last_tcnt;
volatile uchar repeats;

volatile bool timer_on;
volatile bool timeout;

void init_rf();
void init_timer();

void init_rf(){
	
	init_timer();
	
	INPUT(IN_PIN);

	GIMSK = B(INT0);
	MCUCR = 2; 
	
	// OUTPUT(LED1);
	
	sei();
}
void init_timer(){
	TCCR1 = 0;
	TIMSK |= _BV(TOIE1);
	TCNT1 = 0;
}
void start_timer(){
	timer_on = true;
	TCCR1 = PRESCALER;
	TCNT1 = 0;
}
uchar stop_timer(){
	uchar result = TCNT1;
	TCCR1 = 0;	
	timer_on = false;
	
	return result;
}

SIGNAL(INT0_vect){
	if(timer_on){
		
		uchar t = stop_timer();
		
		// if(t > 0 && t < 255){
			
			if(t == last_tcnt){
				repeats++;
				
				// if(repeats >= 5){
					// ON(LED1);
				// }
			}
			else
				repeats=0;
			
			last_tcnt = t;
		// }
	}
	else{
		timeout=0;
		start_timer();
	}
}

ISR(TIM1_OVF_vect){

	stop_timer();
	timeout = true;
}

#endif