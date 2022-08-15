
#include <Macros.h>
// #include "tiny13_serial.h"

volatile ulong val,old_val;
volatile uchar ovf;
volatile uint spd;
volatile uint new_spd;
volatile uchar cchan;
volatile bool connected;

#define PWM_ABS		0
#define PWM_REL_A	4
#define PWM_REL_B	3

#define DEADZONE	33
#define SPD			4 // cnt += SPD

int main (void)
{ 

	INPUT(1);
	OUTPUT(2);
	OUTPUT(PWM_ABS);
	OUTPUT(PWM_REL_A);
	OUTPUT(PWM_REL_B);

	PORTB = B(2);
	
	uint cnt=0;
	
	MCUCR |=/*  B(ISC01) |  */B(ISC00);
	GIMSK |= (1 << INT0);	
	
	TCCR0B=0;
	TCNT0=0;
	TIMSK=B(TOIE0);
	// uart_puts("HI!\n\r\n\r");
	
	sei();
	
	while(!connected);
	
	while(1){
			
				// PORTB = ~( B(PWM_ABS) | B(PWM_REL_A) | B(PWM_REL_B) ) | B(2);
			// if(val != old_val || !cnt){
			if((new_spd != spd) && !(cnt&7)){
				if(new_spd > spd) spd++;
				else spd--;
			}
				// int rspd=(spd-250);

				// uchar cp = (rspd>1) ? 3 : 4;
				uint sr = spd>=250 ? spd-250 : 250-spd;
				sr <<= 1;
				
				if(!cnt){
					
					if(sr > DEADZONE){
						OFF(2);
						if(spd>=250){ 	ON(PWM_REL_B);OFF(PWM_REL_A);}
						else 		{	ON(PWM_REL_A);OFF(PWM_REL_B);}
					}
					
					if((spd) > DEADZONE){
						OFF(2);
						ON(PWM_ABS);
					}
					
					// _delay_us(100);
				}
				
				if(cnt>=spd+DEADZONE) 
					OFF(PWM_ABS);
				if(cnt>=(sr+DEADZONE)) {
					ON(2);
					OFF(PWM_REL_A);
					OFF(PWM_REL_B);
				}
				
				delay_us(280 - (cnt/16));
				// if(cnt>=(rspd<<1)) {OFF(cp); }
				
				
			cnt+=SPD;
			if(cnt>=500) cnt=0;
				// else OFF( PWM_ABS);
				// else OFF(PWM_REL_A);
				// long v = val;
				// while(v){
					// uart_putu((uint16_t)val);
					// uart_puts("\n\r");
					// uart_putc('\n');
					// serial_write((v%10)+62);
					// v/=10;
				// }
				// serial_print("\r\n");
				// old_val=val;
			// }
			
			// _delay_us(64);
			// if(spd < 256)
			
	}
	
	return 0;
}
volatile uchar same=0;
 
ISR(TIMER0_OVF_vect){ ovf++; }
ISR(INT0_vect){
	cli();
	if(READ(1)){
		
		// _delay_us(1200);
		_delay_ms(1);
		TCCR0B=1;
	}
	else{
		TCCR0B=0;
		val = (ovf<<8)+TCNT0;
		int v=(val-760);
		
		// if(!cchan)
		v >>= 4;
		
		// if(v==spd){
			// same++;
			// same&=15;
		// }
		
		// v += spd;
		// v >>= 1;
		
		if(v < 500 &&  v > DEADZONE )
			connected=true;
		
		// if(connected){
			// if(v > 30000) 
				// connected=false;
			new_spd=constrain(v,0,500);
		// }
		// if(cchan < 20)  cchan++;
		// if((v>>4) > 600) cchan=0;
		
	}
	
	TCNT0=ovf=0;
	sei();
}