
#include <Macros.h>
#include <shift_pwm.h>


// #define SINSIZE 64
#include <sintable.h>

#include <avr/sleep.h>
// #include <avr/power.h>

#define HOLDTIME 237
#define FADE_OCR 170

#define MIN_DEL 400		//us
#define DEL_LOOP 130	//us


enum states { SINUS, SOFT, HARD,  FIRE , STANDBY };
uchar state = SINUS;
uchar last_state = SOFT;


void turn_on();
void turn_off();
bool hold(uchar _p);

uchar rmask;
uint press_timer;
uint ain;

volatile uchar r;

int main (void)
{ 
	// OSCCAL += 3;
	
	press_timer = 0;
	uchar cnt=0;
	uchar c_led=4;
	uint sincnt=0;

	BUTTON(0);
	INPUT(4);
	
	shift_init(8);
			
	TCCR1 = _BV(CTC1) | 9;
	TIMSK |= _BV(OCIE1A);
	OCR1A = FADE_OCR;
	
	set_mask(255,0);
	sei();
	
	while(READ(0))
		r = (uchar)lfsr();	
	turn_on();
	
	while (1) {

	
	if(state != STANDBY){
		cnt++;
		cnt&=255;
	
		if(cnt&1){
			ain = adc_read10(2);
			// OCR1A = FADE_OCR+(63-(ain>>4));
		}

		
		if(!(cnt)){
		
			c_led++;
			c_led%=6;
			
			rmask = (r ^ lfsr()) & 31;
			cbi(rmask,c_led);
		}
		
		
	}
	uchar wave = cnt > 127 ? 127 - (cnt&127) : cnt;
	
	_delay_us(MIN_DEL);
	
	for(uint i=(1024-ain);i;i--)
		_delay_us(DEL_LOOP);
	
	uchar l = (cnt%NUM_LEDS);
	
	int sinval = get_sin(sincnt++) + 256;
	sinval >>= 1;
	
	// animate
	switch(state){		
					
		case FIRE:
			
			if(press_timer){
				last_state = SOFT;
				state = HARD;
				break;
			}
			
			if(cnt&3){
				if(get(l) < 6)
					fade_in(l,30 + (r & 63));
				else
					add(l,(int)(7-(r&15)));
			}
			else{
				if((r&7) == 0)
					fade(l,(r&15));
			}		
			
			// if(!(r&15))
				// fade(c_led,wave);
		break;
		case SOFT:
			
			if(press_timer){
				last_state = FIRE;
				state = HARD;
				break;
			}
			
			fade_mask(rmask,wave);
			fade_mask(~rmask,255-wave);
			
			if(!(cnt&7))
				fade_out(c_led,255-(r&127));													

		break;
		
		case SINUS:
			fade_mask(rmask,(uchar)sinval);
			
			if(press_timer)
				state = SOFT;
		break;
		
		case HARD:
			if(!press_timer){
			
				state = last_state;	
				break;
			}
				
			fade_mask(~rmask,MIN(255,press_timer));
			
			if(r&1)
				fade(c_led,wave);
			else
				fade(c_led,255-wave);
			
			if(!(cnt&31)){
			
				fade_mask(rmask,press_timer);
				
				if(!cnt)
					press_timer--;
			}
			
			_delay_ms(40);
			
		break;
		
		case STANDBY:
			
			set_mask(255,0);

			while(READ(0));	
			turn_on();
			
			state = SOFT;
			
		break;

		default:
		break;
	}
			
	if(!READ(0)){
		
	// turn off
		if(hold(0))
			turn_off();
		
	// make bright
		else{
		
			set_mask(rmask,255);
			set_mask(~rmask,160);

			press_timer = 255;
			
		}
		
		
	}
		


	}
	
	return 0;
}

void turn_on(){
	set_mask(255,0);
	uchar m = r & 31;
	// uchar d = 140;
	// uchar v = 255;
	
	for(uchar v=255;v;v>>=1){
		fade_mask(~m,255-v);
		fade_mask(m,v);
		
		delay_ms(50 + (v<<1));
		
		// v -= 3;
		// d -= 2;
		
		// if(i&1)
			m = lfsr() & 31;
	}
	state=FIRE;
}
void turn_off(){

	fade_mask(255,0);
	
	while(!READ(0));		// wait
	fade_out(0,255);
	_delay_ms(1000);

	press_timer=0;	
	state=STANDBY;
}
bool hold(uchar _p){

	uchar hold_timer=HOLDTIME;
	fade_mask(255,100);
	do{
		if(!(hold_timer%40))
			fade_out((hold_timer/5),MAX(20,hold_timer));
		
		if(READ(_p))
			return false;
			
		_delay_ms(6);
			
	}while(--hold_timer);
	
	return true;
}
ISR(TIMER1_COMPA_vect){
	r = 0xff & lfsr();
	blend(r);
}