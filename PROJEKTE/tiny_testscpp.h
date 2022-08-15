
#include <Macros.h>

#define NUM_LEDS 8
#include <shift_fade.h>
#include <sintable.h>
#include <avr/sleep.h>
#include <avr/power.h>
// #include <abuttons.h>

#define YELLOW (0xf0)

volatile uchar press_timer=0;
volatile uchar r=123;

int main (void)
{ 
	// local variables
	uchar cnt=0;
	uchar sinus=0;

	// button
	BUTTON(PB0);
	// BUTTON(PB1);
	OUTPUT(2);
	
	// PRR |= B(PRUSI) | B(PRADC);
	// power_adc_disable();
	// power_usi_disable();
	
	GIMSK |= _BV(PCIE);
	// PCMSK |= 1;
	sei();
	
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	// delay_ms(10);
		// power
	// MCUCR = B(SE);
	
	// while(1);

	// delay_ms(1000);
	
	shift_init();
		
	// blend timer
	TCCR1 = _BV(CTC1) | 11;
	TIMSK |= _BV(OCIE1A);
	OCR1A = 255;
			

	


	// set_mask(0xff,255);
	// fade_mask(0xff,0);
	
	
	

    while (1) {
		
		cnt++;
		cnt&=255;
	
		if(rbi(PINB,0) == false){
			press_timer = 255;
		}		
	
		// uchar r=(uchar)lfsr();
		
		if(press_timer){
			
			if(!(cnt&31))
				press_timer--;		
/* 
			for(int i=0; i<4;i++){
			
				r = (uchar)lfsr();
				fade(i,MAX(30,press_timer - (r&31)));
			}
 */			
			fade_mask(YELLOW,press_timer);			
			randomize((r & ~YELLOW),press_timer>>3,press_timer);
			
			if(press_timer < 100){
				fade_out( (r&3) , press_timer<<1);
			}
			
			uchar d = 8 + (r & 15);
			delay_ms(d);
		}	
		else{
			
			uchar c = cnt;
			
			for(int i=0; i<6;i++){
			
				// r=(lfsr()&15);
				
				sinus = 4 + (get_sin(c) >> 3);
				sinus +=(r&7);
				
				// if(sinus < 50 && !(r&3))
				// if(!(r&7))
				
				
				if(!(r&15))
					fade_out(i,30);
				else
					fade(i,sinus);
				
				c+=32;
				
				delay_ms(50);
			}		
			
			// speed
			r = 16 + (cnt>>3) + (r&15);
			delay_ms(r);
		}

	}
}

ISR(PCINT0_vect){ PORTB |= 0xff; }

ISR(TIMER1_COMPA_vect){
	/* uchar  */r = lfsr() & 255;
		blend(r);
		
}