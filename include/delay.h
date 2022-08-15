#ifndef DELAY_h
#define DELAY_h

#include <util/delay.h>

void delay(unsigned int d){
	if(d)
	do{d--;
	_delay_ms(1);
	}while(d);
}

#endif