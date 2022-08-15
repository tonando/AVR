#ifndef ABUTTONS_h
#define ABUTTONS_h

#include <Macros.h>

#define A_OFFSET	5
uchar n_buttons=0;
uint a_divider=0;
uchar b_channel=0;
// INIT
// anzahl buttons; adc channel
void init_abuttons(uchar _n, uchar _c){
	n_buttons = _n;
	b_channel = _c;
	a_divider = 256 / n_buttons;
		
	ADMUX = _c;       // use ADC2 for input (PB4), MUX bit 0
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);
}

uchar get_button(){
	uchar result = 0;
	
	if(n_buttons) {
		
		uchar a = adc_read(b_channel);
		
		
		// ADMUX = b_channel;       
		// ADCSRA |= (1 << ADSC);         // start ADC measurement
		// while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 
		// uint aval = ADC;
				
		if(a > A_OFFSET)
			result = 1 + (a + A_OFFSET) / a_divider;
		
		
	}

	return result;
}

#endif