				// #include <cstring.h> 


#include "t2313.h"
#include "stdlib.h"
#include "Bot.h"
// #include "Macros.h"
#include "util/delay.h"

bool connected = false;

int main(void){
	DDRB=0xff;
	init_bot();
	
	while(1){
		
		char left=0,right=0;
		
		if(uart_getchar() == 'I')
			PORTB ^= 3;
		
		
		
		if(!connected){
			if(rx_ready()){
				char c = uart_getchar();
				// uart_putchar(c);
				if(c == 'K'){
					ON(LED_B);
					// println("Hi!");
					connected = true;
				}
			}
			
			// OFF(PIN_LED);
			int rand = lfsr_rand();
			left=(char)(rand&0xff);
			right=(char)((rand>>8)&0xff);
			set_motors(left,right);
			// _delay_ms(1200);
		}else{
			
			if(bot_data_ready()){
				ON(LED_A);
				uchar c = bot_read();
				// println(buffer);
				
				// ON(PIN_LED);

				if(c=='X'){
					break_motors();
					// set_motors(0,0);
					// println("STOP");
					// break;
				}
				if(c=='A'){
					set_motors(100,-100);
				}
				else{
					set_motors(buffer[1],buffer[2]);
				}
				
				
				// print("MotorS");
				_delay_ms(BOT_DELAY);
				
			}
			else{
				// OFF(LED_B);
		}
		}
	}
}

