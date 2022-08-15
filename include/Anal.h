#ifndef Anal_h
#define Anal_h

#include <Macros.h>
//#include <inttypes.h>
//#include <avr/io.h>

#define PRE_ANAL 2

static uint8_t A_CHANNEL = 99;

void analInit(uint8_t pre){
     ADMUX = 0x10;
     ADCSRA = (ADCSRA & ~7) | pre; //clear last 4 bits
     sbi(ADCSRA,ADEN);
}

void analChannel(uint8_t channel){
	if(A_CHANNEL == 99)
		analInit(PRE_ANAL);
     
	ADMUX = (ADMUX & 0x1f) | channel;
}

uint16_t analRead(uint8_t channel){
    if(A_CHANNEL != channel)
	   analChannel(channel);
	
	sbi(ADCSRA,ADSC);
	while (rbi(ADCSRA, ADSC));
	
	uint result = ADCL;
	result += (ADCH << 8);
	return result;
	//return (ADCL | (ADCH << 8));
}

#endif
