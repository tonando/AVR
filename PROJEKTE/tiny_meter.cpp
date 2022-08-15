 #define TIMSK TIMSK0
#include <Macros.h>
#include <beep.h>
#define NUM_LEDS 16
// #include <shift_fade.h>

// #include <avr/sleep.h>
// #include <avr/power.h>
#include <abuttons.h>
uint ain=0;

// ISR(TIMER1_COMPA_vect){
		// blend(255);
		
// }

int main ()
{ 
	DDRB = 255;
	ON(3);
	INPUT(4);
	// BUTTON(3);
	init_abuttons(8, 2);
	// shift_init(4);
	// blend timer
	// TCCR1 = _BV(CTC1)| 10;
	// OCR1A = 10;
	// TIMSK |= _BV(OCIE1A);
	// TCCR0A = (1<<COM0B1) | 3; // fast pwm, clear pb1
	// TCCR0B = 1;
	// OCR0B=0;


	sei();
	
	uchar c=0;

    while (1) {
		
		c++;
		
		// OCR0B = (uchar)(b)<<5;
		
		// if(ain < 128)
			// ON(2);
		// else
			// OFF(2);
		
		// beep(2, (440 + ain), 10);
		// uchar b = get_button();
		uchar b = adc_read(2);
		b >>= 4;
		// uchar a = constrain((b << 1),1,15);
	
 	ON(3);
	uchar i=16;
	do{ 
		i--;
	  
		OFF(2); 

		if(i!=b)
			OFF(1);
		else
			ON(1);

		ON(2); 
		 
	}while(i);

	OFF(3);
	OFF(2); 
	ON(3);
		
		// delay_ms(25);

	}
}
