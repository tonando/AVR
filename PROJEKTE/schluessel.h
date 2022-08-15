#include <Macros.h>

#define TIMEOUT 		80

// #define LOCKED
bool locked = false;

volatile uint del_cnt;
volatile uint last_del;
volatile uchar keys;

bool closed = true;
bool awake = true;

// bool end_stop = true;
uchar btn_last;

int main(){
	
	DDRB &= 0xf0;
	PORTB |= 0x0f;
	
	GIMSK = B(PCIE);
	PCMSK = 0x0f;
	// MCUCR = 2; // falling

	TCCR0B = 4;
	TIMSK0 = _BV(TOIE0);
	
	sei();
		
	while(1){
 		
	}
	
	return 0;
}

ISR(PCINT0_vect){
	// sleep_disable();
	del_cnt=0;
	keys++;
}

ISR(TIM0_OVF_vect){

		del_cnt++;

		if(del_cnt > TIMEOUT){
			if(keys >= 4){
				cli();
				ON(4);
				_delay(1000);
				
				OFF(4);
			}
			
			keys=0;
		}
}
