#include <util/delay.h>
#include <Macros.h>

#define TRACKS 8
#define BUTTON_PLAY 0
// #define BUTTON_TAP 4

uchar last_buttons,last_in_byte,in_byte;

typedef struct{
	bool mute : 1;
	uchar pattern[4];
	
}Track;

Track tracks[TRACKS];
uchar sel_track;

uint tempo;
bool playing;

void delay_ms(uint _ms);
void shift_init();

int main (void)	{	
		
	shift_init();
	
	while( 1 ) {

		// Buttons
		// check_button();
if(last_in_byte != in_byte){
	uchar changed = last_in_byte ^ in_byte;
	tracks[0].pattern[0] = changed;
}
		// delay
		delay_ms(tempo >> 4);
	}
	return 0;
}

void delay_ms(uint _ms){
	do{_ms--;
		_delay_ms(1);
	}while(_ms);
}

volatile uchar cnt;

#define DATA  1
#define DATA_IN  4
#define CLOCK 2
#define LATCH 3

void shift_init(){

	DDRB |= _BV(DATA) | _BV(LATCH) | _BV(CLOCK);
	DDRB &= ~_BV(DATA_IN);
	PORTB |= _BV(LATCH);
		
	TCCR0B |= 2;
	TIMSK = _BV(TOIE0); 
	sei();
	
}
/* 
void check_button(){
	uchar b = rbi(PINB,BUTTON_A);

	while(rbi(PINB,BUTTON_B)){
		del_ms++;
		del_ms &= 7;
		
		cli();
		clear(0xff);
		set(del_ms,255);
		_delay_ms(800);
		sei();
	}
	
	// change
	if(b != rbi(last_buttons,BUTTON_A)){
		// down
		if(!b){
			cli();
			
							
			// if(!del_ms){
				clear(0xff);
				mode++;
				mode &= 3;

				_delay_ms(500);
			// }

			// _delay_ms(100);
			sei();
		}
		
	}
	
	last_buttons = b;

}

 */
ISR(TIM0_OVF_vect){
	cnt++;
	
	last_in_byte = in_byte;
	in_byte=0;
	
	OFF(LATCH);

    for(uchar i=0;i<8;i++){
		OFF(DATA);
		OFF(CLOCK); 
      
		if(rbi(tracks[sel_track].pattern[0],i))
			ON(DATA);
    
		if(READ(DATA_IN))
			sbi(in_byte,i);
	
		ON(CLOCK);  
   }
	
	OFF(CLOCK); 
	ON(LATCH);
	
}
