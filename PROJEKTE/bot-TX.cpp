#include "Macros.h"
#include "util/delay.h"
#define TX 1
#include "Bot.h"

#define DATA_PIN PB1
uchar last_pinb;

void init_clock(){
	OSCCAL = 96; // higher value = faster (see Figure 22-42 in data sheet)
	CLKPR = 0x80; // set system clock to 8mhz with no prescale
	CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order
}
void init_buttons(uchar _b){
	DDRB &= ~(_b);
	PORTB |= (_b);
}
uchar check_buttons(){
	uchar result = 0;
	result = (PINB ^ last_pinb) & last_pinb;
	last_pinb = PINB;
	return result;
}
bool check_button(uchar _b){
	uchar b = check_buttons();
	if(rbi(b,_b))
		return true;
	else
		return false;
	
}
int main(void){
	init_clock();
	// adc_setup(2);
	// pwm_setup(1);
	init_buttons(0x01);
	DDRB |= (DATA_PIN); //Set pin PB0 as output (pinMode(0, OUTPUT)
	
	sei();

	while(1){
		uchar b = check_buttons();
	
		if(b){
			if(check_button(0))
				bot_tx(M_LEFT, (uchar)lfsr_rand());
				_delay_ms(5);
				bot_tx(M_RIGHT,(uchar)lfsr_rand());
				_delay_ms(5);
		}
		_delay_ms(50);
		
	}
}
