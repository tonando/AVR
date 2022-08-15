
#include "Anal.h"
#include "math.h"
#include "util/delay.h"

#define LED1 0
#define LED2 1
#define LED3 2

const double PDIV (M_PI / 255.0);

int main(void){
	DDRB = 0xff;//_BV(LED1)|_BV(LED2)|_BV(LED3);
	uchar frame=0;
	// uchar speed=analRead(2)>>2;
	
	uchar leds[3] = {0,0,0};
	
	// sei();

	while(1){
		float tmp = 127 + (sin(PDIV*frame) * 127.0);
		leds[0] = (uchar)tmp;
		leds[2] = 255-leds[0];
		tmp =  127 + (cos(PDIV*frame) * 127.0);
		leds[1] = (uchar)tmp;
		
		for(uchar i=0; i<256; i++){
			PORTB &= ~(_BV(LED1)|_BV(LED2)|_BV(LED3));
			uchar port_out=0;
			
			if(leds[0] > i) port_out |= _BV(LED1);
			else port_out |= _BV(LED3);
			
			if(leds[1] > i) port_out |= _BV(LED2);
			
			PORTB |= port_out;
			_delay_us(64);
		}
		_delay_ms(6);
/* 		delay(speed);
		
		if(frame == 1)
			speed=analRead(2)>>2;
 */			
		frame++;
		frame &= 255;
	}

return 0;
}	