
#include <Macros.h>
#include <beep.h>

// #include <avr/sleep.h>
// #include <avr/power.h>
// #include <abuttons.h>
bool run = false;
uchar steps[16] = {0};

void playNote(uchar _n){
	if(_n >= 48)
		_n = 47;
	
	uint f = pgm_read_word(&freq[_n]);
	beep(0,f,1000);
}

int main ()
{ 
	INPUT(1);
	BUTTON(3);
	BUTTON(4);
	OUTPUT(0);

	// sei();
	// uchar cnote=0;
	uchar cstep=0;
	uchar a=0;

    while (1) {
		if(!READ(3)){
			while(!READ(3));
			run = !run;
			delay_ms(20);
		}
		if(!READ(4)){
			a = adc_read(1);	
			a >>= 3;
			
			steps[cstep] = a;
			// cnote++;
			// if(cnote == 12)
				// cnote = 0;
		}

		if(run){
			
			playNote(steps[cstep]);
			
			cstep++;
			cstep&=15;
			
			delay_ms(200);
		}

	}
}
/* 
ISR(TIMER0_OVF_vect){
	bcount++;
	if(bcount == 16)
		bcount=0;
	
	if(sendmode){		
		if(rbi(buffer,bcount))
			ON(1);
		else
			OFF(1);
	}
	else{
		if(READ(0))
			buffer |= 1;
		
		buffer <<= 1;
		
		test(buffer & 255);
	}
}
 *//* 
ISR (USI_OVF_vect) {
  cbi(USISR,USIOIF);                      // Disable USI         
  // uchar temp = USIDR;
  fbi(PORTB,4);
} */